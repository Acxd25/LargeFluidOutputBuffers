#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "LargeFluidOutputBuffersConfigurationStruct.generated.h"

/* Struct generated from Mod Configuration Asset '/LargeFluidOutputBuffers/Configuration/LargeFluidOutputBuffersConfiguration' */
USTRUCT(BlueprintType)
struct FLargeFluidOutputBuffersConfigurationStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    int32 OutputBufferSizeFluids{};

    UPROPERTY(BlueprintReadWrite)
    bool AutoSetBuffers{};

    UPROPERTY(BlueprintReadWrite)
    bool ExceedPipeMax{};

    /* Retrieves active configuration value and returns object of this struct containing it */
    static FLargeFluidOutputBuffersConfigurationStruct GetActiveConfig(UObject* WorldContext) {
        FLargeFluidOutputBuffersConfigurationStruct ConfigStruct{};
        FConfigId ConfigId{"LargeFluidOutputBuffers", ""};
        if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)) {
            UConfigManager* ConfigManager = World->GetGameInstance()->GetSubsystem<UConfigManager>();
            ConfigManager->FillConfigurationStruct(ConfigId, FDynamicStructInfo{FLargeFluidOutputBuffersConfigurationStruct::StaticStruct(), &ConfigStruct});
        }
        return ConfigStruct;
    }
};

