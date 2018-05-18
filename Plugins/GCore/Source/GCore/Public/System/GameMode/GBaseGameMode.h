// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GSchedulerManager.h"
#include "GBaseGameMode.generated.h"

/**
 * GameMode 基类.
 * GameMode 只是负责告诉GameDirecdtor 状态。让GameDirector来执行业务.
 */
UCLASS()
class GCORE_API AGBaseGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
	
	
public: // 不可以重写.
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	// 子类需要重写.
	virtual void OnBeginPlay();

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason);
};
