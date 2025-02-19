


#include "LFOBRootInstance.h"

void ULFOBRootInstance::DispatchLifecycleEvent(ELifecyclePhase Phase)
{
	Super::DispatchLifecycleEvent(Phase);

    UE_LOG(LogLFOB, Display, TEXT("LFOB TEST"));

	if (Phase == ELifecyclePhase::POST_INITIALIZATION)
    {
        UE_LOG(LogLFOB, Display, TEXT("Attempting CDO on Build_OilRefinery."));
        if (const TSubclassOf<AFGBuildableFactory> BPBuildableOilRefinery = LoadClass<AFGBuildableFactory>(NULL, TEXT("/Game/FactoryGame/Buildable/Factory/OilRefinery/Build_OilRefnery.Build_OilRefnery_C")))
        {
            AFGBuildableFactory* buildableFactoryCDO = BPBuildableOilRefinery.GetDefaultObject();
            buildableFactoryCDO->mFluidStackSizeMultiplier = 2;
			EditedCDOs.Add(buildableFactoryCDO);
            UE_LOG(LogLFOB, Display, TEXT("Build_OilRefinery CDO Applied."));
        }
        else
        {
            UE_LOG(LogLFOB, Error, TEXT("Attempting CDO on Build_OilRefinery FAILED."));
        }
	}
}