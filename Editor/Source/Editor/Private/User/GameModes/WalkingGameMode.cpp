// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkingGameMode.h"
#include "GameDirector.h"

void AWalkingGameMode::OnBeginPlay()
{
	UGameDirector::GetInstance()->ChangeLevel(this, TEXT("WalkingLevel"));

	GSchedulerManager::GetInstance()->AddCondition(TEXT("gameMode"), TEXT("WalkingGameMode"));
}

void AWalkingGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameDirector::GetInstance()->OnEndPlayGameModeEvent(TEXT("WalkingGameMode"));
}
