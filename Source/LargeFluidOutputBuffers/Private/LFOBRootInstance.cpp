#include "LFOBRootInstance.h"

void ULFOBRootInstance::ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe)
{
    // Is manufacturer valid?
    if (not manufacturer)
    {
        UE_LOG(LogLFOB, Error, TEXT("AFGBuildableManufacturer NULLPTR."));
    }
    // Is recipe set?
    else if (not recipe)
    {
        UE_LOG(LogLFOB, Display, TEXT("%s Recipe not set, nothing to do."), *manufacturer->GetName());
    }
    // Try to process
    else
    {
        // Grab the output inventory
        UFGInventoryComponent* inventory = manufacturer->GetOutputInventory();
        // Do we have a valid inventory?
        if (not inventory)
        {
            UE_LOG(LogLFOB, Display, TEXT("%s inventory not set, nothing to do."), *manufacturer->GetName());
        }
        // Inventory valid so contine to process recipe
        else
        {
            // Grab the requested size in m3
            FLargeFluidOutputBuffersConfigurationStruct config = FLargeFluidOutputBuffersConfigurationStruct::GetActiveConfig(manufacturer->GetWorld());
            int32 configSize = config.OutputBufferSizeFluids;
            // Make sure we are never less than 50m3 and never more than 600m3, just a basic sanity check.
            if (configSize < 50)
            {
                configSize = 50;
            }
            else if (configSize > 600)
            {
                configSize = 600;
            }
            // Target Size for buffer in litres.    
            int32 size = 1000 * configSize;

            // Get All Products for the recipe
            TArray<FItemAmount> products = UFGRecipe::GetProducts(recipe);

            // Check those products for being a gas or liquid and set the buffer size if they are
            for (int32 i = 0; i < products.Num(); i++)
            {
                TSubclassOf<class UFGItemDescriptor> itemClass = products[i].ItemClass;
                EResourceForm form = UFGItemDescriptor::GetForm(itemClass);

                if (form == EResourceForm::RF_GAS || form == EResourceForm::RF_LIQUID)
                {
                    FString itemDesc = UFGItemDescriptor::GetItemName(itemClass).ToString();
                    UE_LOG(LogLFOB, Display, TEXT("Found %s Output on '%s' at index %d, setting buffer to %d m3"), (form == EResourceForm::RF_GAS ? TEXT("Gas") : TEXT("Fluid")), *itemDesc, i, configSize);
                    inventory->AddArbitrarySlotSize(i, size);
                }
            }
        }
    }
}

void ULFOBRootInstance::DispatchLifecycleEvent(ELifecyclePhase Phase)
{
	Super::DispatchLifecycleEvent(Phase);
	if (Phase == ELifecyclePhase::POST_INITIALIZATION)
    {
        if (!WITH_EDITOR) 
        {
            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::SetRecipe,
                [this](AFGBuildableManufacturer* self, TSubclassOf< class UFGRecipe > recipe)
                {
                    UE_LOG(LogLFOB, Display, TEXT("SetRecipe Called on %s, Checking Output Buffers."), *self->GetName());
                    ProcessOutputBuffer(self, recipe);
                });

            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::BeginPlay,
                [this](AFGBuildableManufacturer* self)
                {
                    UE_LOG(LogLFOB, Display, TEXT("BeginPlay Called on %s, Checking Output Buffers."), *self->GetName());
                    TSubclassOf< class UFGRecipe > recipe = self->GetCurrentRecipe();
                    ProcessOutputBuffer(self, recipe);
                });
        }
	}
}