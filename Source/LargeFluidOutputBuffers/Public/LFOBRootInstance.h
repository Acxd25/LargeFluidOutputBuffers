

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
			// Generic variables
			Direction direction = Direction::OUTPUT;
			float productionBoost = 1.f;  // Default to 1x, i.e. no slooping or we are an input

			// Fluid Variables
			bool autoSetFluidBuffers = false;
			bool exceedFluidMax = false;
			int32 fixedBufferSize = 50;   // Default to game normal
			mutable int32 sizeInCubicMetres = 50; // Default to game normal
			mutable int32 sizeInLitres = 50000;   // Default to game normal
			
			// Solid variables
			bool autoSetSolidBuffers = false;
			bool allowBelowMinStack = false;
			int32 fixedSolidStackSize = 50;
			mutable int32 solidStackSize = 50;
		};

		// Actually process the buffers, do not call any of these directly via any friend mechanisms.  Use
		// the public methods
		void ProcessOutputBuffersInternal(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe, float productionBoost);
		void ProcessInputBuffersInternal(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe);
		void ProcessInventory(UFGInventoryComponent* inventory, const ProcessingParameters& parameters, const TSubclassOf<class UFGRecipe> recipe);
		void ProcessFixedFluidBufferSize(const ProcessingParameters& parameters);
		void ProcessDynamicFluidBufferSize(FItemAmount item, const ProcessingParameters& parameters);
		void ProcessFixedSolidBufferSize(const ProcessingParameters& parameters);
		void ProcessDynamicSolidBufferSize(FItemAmount item, const ProcessingParameters& parameters);
		// Recalculate if we can produce products
		void RecaculateCanProduceOutput(AFGBuildableManufacturer* manufacturer);
};
