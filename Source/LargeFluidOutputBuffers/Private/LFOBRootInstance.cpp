#include "LFOBRootInstance.h"

void ULFOBRootInstance::ProcessOutputBuffer(AFGBuildableManufacturer* self, TSubclassOf< class UFGRecipe > recipe)
{
    if (not recipe)
    {
        UE_LOG(LogLFOB, Display, TEXT("Recipe not set, nothing to do."));
    }
    else
    {
        UFGInventoryComponent* inventory = self->GetOutputInventory();
        FInventoryItem item;
        FInventoryStack stack;

        FLargeFluidOutputBuffersConfigurationStruct config = FLargeFluidOutputBuffersConfigurationStruct::GetActiveConfig(self->GetWorld());
        int32 size = 1000 * config.OutputBufferSizeFluids;
        TArray< FItemAmount > products = UFGRecipe::GetProducts(recipe);

        for (int32 i = 0; i < products.Num(); i++)
        {
            TSubclassOf<class UFGItemDescriptor> itemClass = products[i].ItemClass;
            EResourceForm form = UFGItemDescriptor::GetForm(itemClass);
            FString itemDesc = UFGItemDescriptor::GetItemName(itemClass).ToString();
            if (form == EResourceForm::RF_GAS || form == EResourceForm::RF_LIQUID)
            {
                UE_LOG(LogLFOB, Display, TEXT("Found %s Output '%s' at index %d, increasing buffer to %d m3"), (form == EResourceForm::RF_GAS ? TEXT("Gas") : TEXT("Fluid")), *itemDesc, i, config.OutputBufferSizeFluids);
                inventory->AddArbitrarySlotSize(i, size);
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
                [](AFGBuildableManufacturer* self, TSubclassOf< class UFGRecipe > recipe)
                {
                    UE_LOG(LogLFOB, Display, TEXT("SetRecipe Called on %s, Checking Output Buffers."), *self->GetName());
                    ProcessOutputBuffer(self, recipe);
                });

            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::BeginPlay,
                [](AFGBuildableManufacturer* self)
                {
                    UE_LOG(LogLFOB, Display, TEXT("BeginPlay Called on %s, Checking Output Buffers."), *self->GetName());
                    TSubclassOf< class UFGRecipe > recipe = self->GetCurrentRecipe();
                    ProcessOutputBuffer(self, recipe);
                });
        }
	}
}