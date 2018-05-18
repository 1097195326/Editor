// Fill out your copyright notice in the Description page of Project Settings.

#include "LoginGameMode.h"
#include "GameDirector.h"
#include "GModuleManager.h"
#include "BaseLogicScene.h"
#include "Editor.h"
#include "StyleR.h"
#include "MainModule.h"



void ALoginGameMode::OnBeginPlay()
{
	// 打开逻辑场景.
	UGameDirector::GetInstance()->OpenLogicScene(TEXT("LogicLoginScene"), this);

	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	GSchedulerManager::GetInstance()->AddCondition(TEXT("gameMode"), TEXT("LoginGameMode"));
}

void ALoginGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameDirector::GetInstance()->CloseCurrentLogicScene();
}
// 
// void ALoginGameMode::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
// {
// 	PlayerInputComponent->BindAction("PasswordLogin", IE_Pressed, this, &ALoginGameMode::PasswordLogin);
// }
// 
// void ALoginGameMode::PasswordLogin()
// {
// 
// }
