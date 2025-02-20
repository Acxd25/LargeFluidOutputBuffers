


#include "LFOBRootInstance.h"

void ULFOBRootInstance::DispatchLifecycleEvent(ELifecyclePhase Phase)
{
	Super::DispatchLifecycleEvent(Phase);
	if (Phase == ELifecyclePhase::POST_INITIALIZATION)
    {
        if (!WITH_EDITOR) 
        {
            //AFGBuildableManufacturer* SampleObject = GetMutableDefault<AFGBuildableManufacturer>();
            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::SetRecipe,
                [](AFGBuildableManufacturer* self, TSubclassOf< class UFGRecipe > recipe)
                {
                    UE_LOG(LogLFOB, Display, TEXT("Recipe Changed, adjusting Output Buffer."));
                    UFGInventoryComponent* inventory = self->GetOutputInventory();
                    FInventoryItem item;
                    FInventoryStack stack;
                    
                    for (int32 i = 0; i < inventory->GetSizeLinear(); i++)
                    {
                        if (inventory->GetStackFromIndex(i, stack))
                        {
                            UE_LOG(LogLFOB, Display, TEXT("Check Stack %d"), i);
                            TSubclassOf<class UFGItemDescriptor> itemClass = stack.Item.GetItemClass();
                            if (not itemClass)
                            {
                                UE_LOG(LogLFOB, Display, TEXT("itemClass NULLPTR"));
                            }
                            else
                            {
                                UE_LOG(LogLFOB, Display, TEXT("Check Stack %s"), *itemClass->GetDescription());
                            }
                            
                            UE_LOG(LogLFOB, Display, TEXT("Size = %d."), stack.Item.GetItemStackSize());

                            //EResourceForm form = UFGItemDescriptor::GetForm(itemClass);
                            //if (form == EResourceForm::RF_GAS || form == EResourceForm::RF_LIQUID)
                            //{
                                UE_LOG(LogLFOB, Display, TEXT("Found GAS/FLUID"));
                                // Fix that Stack Size!
                                inventory->AddArbitrarySlotSize(i, 100000);
                            //}
                        }
                    }
                });
        }


        /*UE_LOG(LogLFOB, Display, TEXT("Attempting CDO on Build_OilRefinery."));
        if (const TSubclassOf<AFGBuildableFactory> BPBuildableOilRefinery = LoadClass<AFGBuildableFactory>(NULL, TEXT("/Game/FactoryGame/Buildable/Factory/OilRefinery/Build_OilRefinery.Build_OilRefinery_C")))
        {
            AFGBuildableFactory* buildableFactoryCDO = BPBuildableOilRefinery.GetDefaultObject();
            buildableFactoryCDO->mFluidStackSizeMultiplier = 2;
			EditedCDOs.Add(buildableFactoryCDO);
            UE_LOG(LogLFOB, Display, TEXT("Build_OilRefinery CDO Applied."));
        }
        else
        {
            UE_LOG(LogLFOB, Error, TEXT("Attempting CDO on Build_OilRefinery FAILED."));
        }*/
	}
}