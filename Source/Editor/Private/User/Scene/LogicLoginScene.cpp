// Fill out your copyright notice in the Description page of Project Settings.

#include "LogicLoginScene.h"
#include "LoadDataModule.h"
#include "GModuleManager.h"
#include "GameDirector.h"
#include "MsgCenter.h"

static int8 _LoadHomeInfoIndex_ = 0;

GREGISTER(LogicLoginScene)
LogicLoginScene::LogicLoginScene()
{

}

LogicLoginScene::~LogicLoginScene()
{

}

void LogicLoginScene::OnLoad()
{
	OpenUIController(TEXT("LoginUIController"));

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LoadDataModule::m_loadUserPattern, this, &LogicLoginScene::OnDownloadUserPatternSuccess);

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LoadDataModule::m_loadHomeInfoBikiniFinish, this, &LogicLoginScene::OnLoadHomeFinish);
}

void LogicLoginScene::OnRelease()
{
	_LoadHomeInfoIndex_ = 0;
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LoadDataModule::m_loadUserPattern, this, &LogicLoginScene::OnDownloadUserPatternSuccess);

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LoadDataModule::m_loadHomeInfoBikiniFinish, this, &LogicLoginScene::OnLoadHomeFinish);
}


void LogicLoginScene::JoinMainScene()
{
	// 先下载用户花型.
	LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	mLoadDataModule->LoadHomeInfoBikini();

}

void LogicLoginScene::OnDownloadUserPatternSuccess(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

		mLoadDataModule->LoadHomeInfoBikini();
	}
	else
	{
		
	}
}


void LogicLoginScene::OnLoadHomeFinish(msg_ptr _msg)
{
	MsgLocal * mLocalMsg = _msg->ConvertMsg<MsgLocal>();

	EnumMsgResponseErrorCode mErrorCode = mLocalMsg->GetMsgErrorCode();

	if (mErrorCode == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 下载完成以后再跳转场景.
		if (_LoadHomeInfoIndex_ == 0)
		{
			_LoadHomeInfoIndex_ = 1;
			LoadDataModule * mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");
			mLoadDataModule->LoadHomeInfoSport();
			return;
		}
		UGameDirector::GetInstance()->OpenLevel(TEXT("MainLevel"));
	}
	else
	{

	}
}