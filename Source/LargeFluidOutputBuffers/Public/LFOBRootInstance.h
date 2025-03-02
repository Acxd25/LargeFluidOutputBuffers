

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
	protected:
		void ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer);
		void ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe);
		void ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer, float productionBoost);
		void ProcessOutputBuffer(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe, float productionBoost);
	public:
	virtual void DispatchLifecycleEvent(ELifecyclePhase phase) override;
};
