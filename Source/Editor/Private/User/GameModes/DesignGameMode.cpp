// Fill out your copyright notice in the Description page of Project Settings.

#include "DesignGameMode.h"
#include "GameDirector.h"

void ADesignGameMode::OnBeginPlay()
{
	UGameDirector::GetInstance()->OpenLogicScene(TEXT("LogicDesignScene"), this, TEXT("DesignLevel"));

	GSchedulerManager::GetInstance()->AddCondition(TEXT("gameMode"), TEXT("DesignGameMode"));
}

void ADesignGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameDirector::GetInstance()->OnEndPlayGameModeEvent(TEXT("DesignGameMode"));
	//UGameDirector::GetInstance()->CloseCurrentLogicScene();
}

