// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameMode/GBaseGameMode.h"
#include "LoginGameMode.generated.h"

/*
 *
 * 
 */
UCLASS()
class EDITOR_API ALoginGameMode : public AGBaseGameMode
{
	GENERATED_BODY()
	
	
protected:
	// 子类需要重写. 
	virtual void OnBeginPlay() override;

	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PasswordLogin();
};
