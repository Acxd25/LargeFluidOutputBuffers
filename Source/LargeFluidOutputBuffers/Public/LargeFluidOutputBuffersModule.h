// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Logging/StructuredLog.h"
#include "FGBuildableManufacturer.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLFOB, Log, All);

class FLargeFluidOutputBuffersModule : public FDefaultGameModuleImpl
{

public:
	// Buffer Process functions
	UFUNCTION() void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer);
	UFUNCTION() void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe);
	UFUNCTION() void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, const float productionBoost);
	UFUNCTION() void ProcessOutputBuffers(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe, const float productionBoost);
	UFUNCTION() void ProcessInputBuffers(AFGBuildableManufacturer* manufacturer);
	UFUNCTION() void ProcessInputBuffers(AFGBuildableManufacturer* manufacturer, const TSubclassOf< class UFGRecipe > recipe);

	virtual void StartupModule() override;

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
