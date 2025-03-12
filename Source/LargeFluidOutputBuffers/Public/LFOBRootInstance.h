

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
		enum Direction
		{
			INPUT,
			OUTPUT
		};

		struct ProcessingParameters
		{
			Direction direction = Direction::OUTPUT;
			bool autoSetBuffers = false;
			bool exceedMax = false;
			int32 fixedBufferSize = 50;   // Default to game normal
			int32 sizeInCubicMetres = 50; // Default to game normal
			int32 sizeInLitres = 50000;   // Default to game normal
			float productionBoost = 1.f;  // Default to 1x, i.e. no slooping or we are an input
		};

		// Actually process the buffers, do not call any of these directly via any friend mechanisms.  Use
		// the public methods
		void ProcessOutputBuffersInternal(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe, float productionBoost);
		void ProcessInputBuffersInternal(AFGBuildableManufacturer* manufacturer, TSubclassOf< class UFGRecipe > recipe);
		void ProcessInventory(UFGInventoryComponent* inventory, ProcessingParameters& parameters, TSubclassOf<class UFGRecipe> recipe);
		void ProcessFixedBufferSize(ProcessingParameters& parameters);
		void ProcessDynamicBufferSize(FItemAmount item, ProcessingParameters& parameters);
		// Recalculate if we can produce products
		void RecaculateCanProduceOutput(AFGBuildableManufacturer* manufacturer);
};
