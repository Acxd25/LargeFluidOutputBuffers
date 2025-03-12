

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

	public:
		// Buffer Process functions
		void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer);
		void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe);
		void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, float productionBoost);
		void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe, float productionBoost);
		void ProcessInputBuffers(AFGBuildableManufacturer* manufacturer);
		void ProcessInputBuffers(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe);

		// Lifecycle handler
		virtual void DispatchLifecycleEvent(ELifecyclePhase phase) override;

	private:
		// Actually process the buffers, do not call directly via any friend mechanisms.  Use
		// the public methods
		void ProcessOutputBuffersInternal(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe, float productionBoost);
		void ProcessInputBuffersInternal(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe);
		// Recalculate if we can produce products
		void RecaculateCanProduceOutput(AFGBuildableManufacturer* manufacturer);
};
