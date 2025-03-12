

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
		void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe);
		void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, const float productionBoost);
		void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe, const float productionBoost);
		void ProcessInputBuffers(AFGBuildableManufacturer* manufacturer);
		void ProcessInputBuffers(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe);

		// Lifecycle handler
		virtual void DispatchLifecycleEvent(ELifecyclePhase phase) override;

	private:
		struct ProcessingParameters
		{
			enum Direction
			{
				INPUT,
				OUTPUT
			};

			// Generic variables
			Direction direction = Direction::OUTPUT;
			float productionBoost = 1.f;  // Default to 1x, i.e. no slooping or we are an input

			// Fluid Variables
			bool autoSetFluidBuffers = false;
			bool exceedFluidMax = false;
			int32 fixedFluidBufferSize = 50;   // Default to game normal
			mutable int32 sizeInCubicMetres = 50; // Default to game normal, this can change even if we are declared const.
			mutable int32 sizeInLitres = 50000;   // Default to game normal, this can change even if we are declared const.
			
			// Solid variables -- Currently perma disabled as no GUI to turn it on.
			bool processSolids = false;
			bool autoSetSolidBuffers = false;
			bool allowBelowMinStack = false;
			int32 fixedSolidStackSize = 50;
			mutable int32 solidStackSize = 50;  // Default to game normal, this can change even if we are declared const.
		};

		// Actually process the buffers, do not call any of these directly via any friend mechanisms.  Use the public methods
		void ProcessOutputBuffersInternal(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe,const float productionBoost);
		void ProcessInputBuffersInternal(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe);
		void ProcessInventory(UFGInventoryComponent* inventory, const ProcessingParameters& parameters, const TSubclassOf<class UFGRecipe> recipe);

		// Recalculate if we can produce products based on our available output space.
		void RecaculateCanProduceOutput(AFGBuildableManufacturer* manufacturer);

		// All the following helper functions use the ProcessingParameters to determine correct values, they will only change the mutable components
		// and cannot change anything else.  This is fully documented in the structure ProcessingParameters.
		static inline void ProcessFixedFluidBufferSize(const ProcessingParameters& parameters);
		static inline void ProcessDynamicFluidBufferSize(const FItemAmount item, const ProcessingParameters& parameters);
		static inline void ProcessFixedSolidBufferSize(const ProcessingParameters& parameters);
		static inline void ProcessDynamicSolidBufferSize(const FItemAmount item, const ProcessingParameters& parameters);
};
