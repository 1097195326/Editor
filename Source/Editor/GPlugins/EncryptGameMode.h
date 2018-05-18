// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameMode/GBaseGameMode.h"
#include "EncryptGameMode.generated.h"

/**
 * 加密所有的config配置文件.
 */
UCLASS()
class EDITOR_API AEncryptGameMode : public AGBaseGameMode
{
	GENERATED_BODY()
	
	
protected:
	// 子类需要重写. 
	virtual void OnBeginPlay() override;

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = Walk)
	static FString GetEncodeStr(FString _content);
	
};
