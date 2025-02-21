

#pragma once

#include "CoreMinimal.h"
#include "Module/GameInstanceModule.h"
#include "LargeFluidOutputBuffers.h"
#include "FGBuildableFactory.h"
#include "FGBuildableManufacturer.h"
#include "FGInventoryLibrary.h"
#include "Patching/NativeHookManager.h"
#include "LargeFluidOutputBuffersConfigurationStruct.h"

#include "LFOBRootInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LARGEFLUIDOUTPUTBUFFERS_API ULFOBRootInstance : public UGameInstanceModule
{
	GENERATED_BODY()

	private:
	/* Marked as UPROPERTY because it holds CDO edits */
	UPROPERTY()
	TSet<UObject*> EditedCDOs;

	protected:
		static void ProcessOutputBuffer(AFGBuildableManufacturer* self, TSubclassOf< class UFGRecipe > recipe);
	public:
	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;
};
