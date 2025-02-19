

#pragma once

#include "CoreMinimal.h"
#include "Module/GameInstanceModule.h"
#include "LargeFluidOutputBuffers.h"
#include "FGBuildableFactory.h"
#include "FGInventoryLibrary.h"

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

	public:
	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;
};
