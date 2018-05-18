#include "LogicUpdateScene.h"
#include "GModuleManager.h"
#include "MsgCenter.h"
#include "UpdateResourceModule.h"
#include "GameDirector.h"
#include "HotLoadManager.h"
#include "UpdateUIController.h"
#include "ServerLogTool.h"
GREGISTER(LogicUpdateScene)
LogicUpdateScene::LogicUpdateScene()
{

}

LogicUpdateScene::~LogicUpdateScene()
{

}

void LogicUpdateScene::OnLoad()
{
	OpenUIController(TEXT("UpdateUIController"));
	
	//请求强制更新服务器.
	StartForcedUpdate();


	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_DownloadPak_Finished, this, &LogicUpdateScene::OnDownloadPakSuccess);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LoadDataModule::m_downloadDefaultDataFinish, this, &LogicUpdateScene::OnDownloadDefaultDataSuccess);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LoadDataModule::m_initXmlFinish, this, &LogicUpdateScene::OnInitXmlSuccess);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LoadDataModule::m_loadDesignTypeFinish, this, &LogicUpdateScene::OnLoadTypeFinish);

}

void LogicUpdateScene::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LoadDataModule::m_loadDesignTypeFinish, this, &LogicUpdateScene::OnLoadTypeFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LoadDataModule::m_initXmlFinish, this, &LogicUpdateScene::OnInitXmlSuccess);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_DownloadPak_Finished, this, &LogicUpdateScene::OnDownloadPakSuccess);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LoadDataModule::m_downloadDefaultDataFinish, this, &LogicUpdateScene::OnDownloadDefaultDataSuccess);
}

void LogicUpdateScene::StartForcedUpdate()
{
	GModuleManager::GetInstance()->GetModule<UpdateResourceModule>("UpdateResourceModule")->RequestForcedUpdateServer();
}

void LogicUpdateScene::OnFinishedForcedUpdate()
{
     LoadDataModule * mModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");
     mModule->InitDownloadData();
//    DownloadPak();
//#if PLATFORM_ANDROID
//   DownloadPak();
//#elif PLATFORM_WINDOWS
//   DownloadPak();
//#else
//   LoadDataModule * mModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");
//   mModule->InitDownloadData();
//#endif

}


void LogicUpdateScene::DownloadPak()
{
	UpdateUIController *mUpdateUIController = (UpdateUIController*)GetCurrentUIController();
	mUpdateUIController->DownloadPak();
}

void LogicUpdateScene::DownloadDefaultData()
{
	UpdateUIController *mUpdateUIController=(UpdateUIController*)GetCurrentUIController();
	mUpdateUIController->DownloadDefaultData();
}



void LogicUpdateScene::OnDownloadPakSuccess(msg_ptr _msg)
{
	LoadDataModule * mModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");
	mModule->InitDownloadData();
}

void LogicUpdateScene::OnDownloadDefaultDataSuccess(msg_ptr _msg)
{
	LoadDataModule * mModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");

	mModule->LoadDesignTypeT();

	
}

void LogicUpdateScene::OnInitXmlSuccess(msg_ptr _msg)
{
	DownloadDefaultData();
}


void LogicUpdateScene::OnLoadTypeFinish(msg_ptr _msg)
{
	UGameDirector::GetInstance()->OpenLevel(TEXT("LoginLevel"));
}
