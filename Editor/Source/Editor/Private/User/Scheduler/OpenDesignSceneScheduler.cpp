// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDesignSceneScheduler.h"
#include "MainModule.h"
#include "LogicMainScene.h"
#include "GModuleManager.h"
GREGISTER(OpenDesignSceneScheduler)

OpenDesignSceneScheduler::OpenDesignSceneScheduler()
{
}

OpenDesignSceneScheduler::~OpenDesignSceneScheduler()
{
}

void OpenDesignSceneScheduler::OnLoad()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 1022, this, &OpenDesignSceneScheduler::ClickCallBack);
}

void OpenDesignSceneScheduler::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 1022, this, &OpenDesignSceneScheduler::ClickCallBack);
}

void OpenDesignSceneScheduler::Excute(TMap<FString, FString> & _params)
{
	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	if (_params.Contains(TEXT("styleid")))
		mMainModule->GetSystemStyleDataByID(FCString::Atoi(*_params[TEXT("styleid")]));
}

void OpenDesignSceneScheduler::ClickCallBack(msg_ptr _msg)
{
	UE_LOG(LogTemp, Log, TEXT("ZHX : UStyleSpecialIcon::ClickCallBack"));
	MainModule * mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");

	TArray<SelectCellData> mArr = mMainModule->m_Sys_SelectCell_Dats_ById;
	SelectCellData data = mArr[0];

	LogicMainScene *mLogicMainScene = (LogicMainScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

	mLogicMainScene->BeginJoinDesignLevel(data.m_Rid, data.m_Tid, EnumDesignType::DESIGN_BY_SYS_STYLE);

	GRelease();
}