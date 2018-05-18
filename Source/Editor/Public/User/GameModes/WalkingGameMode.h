// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameMode/GBaseGameMode.h"
#include "WalkingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API AWalkingGameMode : public AGBaseGameMode
{
	GENERATED_BODY()
public:
	void OnBeginPlay();
	void OnEndPlay(const EEndPlayReason::Type EndPlayReason);
	
};
