// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameMode/GBaseGameMode.h"
#include "DecryptGameMode.generated.h"

/**
 * 解密所有的配置文件.
 */
UCLASS()
class EDITOR_API ADecryptGameMode : public AGBaseGameMode
{
	GENERATED_BODY()
	
	


protected:
	// 子类需要重写. 
	virtual void OnBeginPlay() override;

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = Walk)
		static FString GetDecyptStr(FString _content);
};
