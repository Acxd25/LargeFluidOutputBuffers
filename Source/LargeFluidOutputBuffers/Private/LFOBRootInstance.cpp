#include "LFOBRootInstance.h"

void ULFOBRootInstance::ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer)
{
    // Is manufacturer valid?  
    if (not IsValid(manufacturer))
    {
        UE_LOG(LogLFOB, Error, TEXT("AFGBuildableManufacturer IsValid Failed."));
        return;
    }

	ProcessOutputBufferInternal(manufacturer, manufacturer->GetCurrentRecipe(), manufacturer->GetCurrentProductionBoost());
}

void ULFOBRootInstance::ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer, TSubclassOf<class UFGRecipe> recipe)
{
    // Is manufacturer valid?  
    if (not IsValid(manufacturer))
    {
        UE_LOG(LogLFOB, Error, TEXT("AFGBuildableManufacturer IsValid Failed."));
        return;
    }

	ProcessOutputBufferInternal(manufacturer, recipe, manufacturer->GetCurrentProductionBoost());
}

void ULFOBRootInstance::ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer, float productionBoost)
{
    // Is manufacturer valid?  
    if (not IsValid(manufacturer))
    {
        UE_LOG(LogLFOB, Error, TEXT("AFGBuildableManufacturer IsValid Failed."));
        return;
    }

	ProcessOutputBufferInternal(manufacturer, manufacturer->GetCurrentRecipe(), productionBoost);
}

void ULFOBRootInstance::ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe, float productionBoost)
{
	// Is manufacturer valid?  
	if (not IsValid(manufacturer))
	{
		UE_LOG(LogLFOB, Error, TEXT("AFGBuildableManufacturer IsValid Failed."));
		return;
	}

	ProcessOutputBufferInternal(manufacturer, recipe, productionBoost);
}

// DO NOT CALL DIRECTLY.
void ULFOBRootInstance::ProcessOutputBufferInternal(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe, float productionBoost)
{
	// Is recipe set?
	if (not IsValid(recipe))
	{
		UE_LOG(LogLFOB, Display, TEXT("%s Recipe not set, nothing to do."), *manufacturer->GetName());
		return;
	}

	// Grab the output inventory
	UFGInventoryComponent* inventory = manufacturer->GetOutputInventory();
	// Do we have a valid output inventory?  This should be impossible for a well defined machine, but let's be safe.
	if (not IsValid(inventory))
	{
		UE_LOG(LogLFOB, Error, TEXT("%s output inventory IsValid Failed"), *manufacturer->GetName());
		return;
	}

	// Grab the Configuration for this mod.
	FLargeFluidOutputBuffersConfigurationStruct config = FLargeFluidOutputBuffersConfigurationStruct::GetActiveConfig(GetWorld());

	// Should we auto calculate the correct buffer sizes?
	bool autoSetBuffers = config.DynamicSettings.AutoSetBuffers;
	// Can we exceed 600m3 during an autoset?
	bool exceedMax = config.DynamicSettings.ExceedPipeMax;

	int32 sizeInCubicMetres;
	int32 sizeInLitres;

	if (!autoSetBuffers)
	{
		// Grab the requested size in m3
		sizeInCubicMetres = config.OutputBufferSizeFluids;
		// Make sure we are never less than 50m3 and never more than 600m3, just a basic sanity check.
		if (sizeInCubicMetres < 50)
		{
			sizeInCubicMetres = 50;
		}
		else if (sizeInCubicMetres > 600)
		{
			sizeInCubicMetres = 600;
		}
		// Target Size for buffer in litres.    
		sizeInLitres = 1000 * sizeInCubicMetres;
	}

	FString mode = autoSetBuffers ? exceedMax ? TEXT("DYNAMIC+") : TEXT("DYNAMIC") : TEXT("FIXED");

	// Get All Products for the recipe
	TArray<FItemAmount> products = UFGRecipe::GetProducts(recipe);

	// Check those products for being a gas or liquid and set the buffer size if they are
	for (int32 i = 0; i < products.Num(); i++)
	{
		TSubclassOf<class UFGItemDescriptor> itemClass = products[i].ItemClass;
		if (IsValid(itemClass))
		{
			EResourceForm form = UFGItemDescriptor::GetForm(itemClass);

			if (form == EResourceForm::RF_GAS || form == EResourceForm::RF_LIQUID)
			{
				FString itemDesc = UFGItemDescriptor::GetItemName(itemClass).ToString();
				// Are we supposed to automatically set the buffer size?  If so calculate the correct value.
				if (autoSetBuffers)
				{
					sizeInLitres = ceil(products[i].Amount * 2 * productionBoost);
					// Ensure we are never less than 50m3
					if (sizeInLitres < 50000)
					{
						sizeInLitres = 50000;
					}
					// Only do this check if we are not exceeding 600m3
					else if (!exceedMax && sizeInLitres > 600000)
					{
						sizeInLitres = 600000;
					}
					sizeInCubicMetres = sizeInLitres / 1000;
				}
				UE_LOG(LogLFOB, Display, TEXT("[MODE = %s] Found %s Output '%s' at index %d, setting buffer to %d m3"), *mode, form == EResourceForm::RF_GAS ? TEXT("Gas") : TEXT("Fluid"), *itemDesc, i, sizeInCubicMetres);
				inventory->AddArbitrarySlotSize(i, sizeInLitres);
			}
		}
	}
}

void ULFOBRootInstance::DispatchLifecycleEvent(ELifecyclePhase phase)
{
	Super::DispatchLifecycleEvent(phase);
	if (phase == ELifecyclePhase::POST_INITIALIZATION)
    {
        if (!WITH_EDITOR) 
        {
            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::SetRecipe,
                [&](AFGBuildableManufacturer* self, TSubclassOf< class UFGRecipe > recipe)
                {
                    UE_LOG(LogLFOB, Display, TEXT("SetRecipe Called on %s, Checking Output Buffers."), *self->GetName());
                    ProcessOutputBuffer(self, recipe);
                });

            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::BeginPlay,
                [&](AFGBuildableManufacturer* self)
                {
                    UE_LOG(LogLFOB, Display, TEXT("BeginPlay Called on %s, Checking Output Buffers."), *self->GetName());
                    ProcessOutputBuffer(self);
                });

            SUBSCRIBE_UOBJECT_METHOD_AFTER(AFGBuildableManufacturer, AFGBuildableManufacturer::SetCurrentProductionBoost,
                [&](AFGBuildableManufacturer* self, float newProductionBoost)
                {
                    UE_LOG(LogLFOB, Display, TEXT("SetCurrentProductionBoost Called on %s, Checking Output Buffers."), *self->GetName());
                    ProcessOutputBuffer(self, newProductionBoost);
                });
        }
	}
}