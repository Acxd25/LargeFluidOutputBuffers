


#include "LFOBRootInstance.h"

void ULFOBRootInstance::ProcessOutputBuffer(AFGBuildableManufacturer* self, TSubclassOf< class UFGRecipe > recipe)
{
    UFGInventoryComponent* inventory = self->GetOutputInventory();
    FInventoryItem item;
    FInventoryStack stack;

    TArray< FItemAmount > products = UFGRecipe::GetProducts(recipe);
    for (int32 i = 0; i < products.Num(); i++)
    {
        TSubclassOf<class UFGItemDescriptor> itemClass = products[i].ItemClass;
        EResourceForm form = UFGItemDescriptor::GetForm(itemClass);
        FString itemDesc = UFGItemDescriptor::GetItemName(itemClass).ToString();
        if (form == EResourceForm::RF_GAS || form == EResourceForm::RF_LIQUID)
        {
            UE_LOG(LogLFOB, Display, TEXT("Output Found GAS/FLUID '%s' at index %d, increasing buffer to 100m3"), *itemDesc, i);
            inventory->AddArbitrarySlotSize(i, 100000);
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
                    UE_LOG(LogLFOB, Display, TEXT("Recipe Changed, Checking Output Buffer."));
                    ProcessOutputBuffer(self, recipe);
                });

            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::BeginPlay,
                [](AFGBuildableManufacturer* self)
                {
                    UE_LOG(LogLFOB, Display, TEXT("BeginPlay Called, Checking Output Buffer."));
                    TSubclassOf< class UFGRecipe > recipe = self->GetCurrentRecipe();
                    ProcessOutputBuffer(self, recipe);
                });
        }
	}
}