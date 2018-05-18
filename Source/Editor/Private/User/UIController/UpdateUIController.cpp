// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdateUIController.h"
#include "UpdateResourceModule.h"
#include "MsgCenter.h"
#include "HotLoadManager.h"
#include "GModuleManager.h"
#include "GameDirector.h"
#include "LogicUpdateScene.h"
#include "Win_HotUpdateUMG.h"
// 反射.
GREGISTER(UpdateUIController)

UpdateUIController::UpdateUIController()
{
	m_HotUpdateUMG = nullptr;
}

UpdateUIController::~UpdateUIController()
{
}

void UpdateUIController::OnLoad()
{
	// 初始化要打开的根UI.
	SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("ForcedUpdateUMG")));
	m_HotUpdateUMG =(UHotUpdateUMG*)UUIManager::GetInstance()->OpenUI(TEXT("HotUpdateUMG"));
	m_HotUpdateUMG->AddToViewport();
	GetRootUI()->AddToViewport();
	 
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LogicUpdateScene::m_DownloadPak_Progress, this, &UpdateUIController::OnDownloadPakProgress);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LogicUpdateScene::m_DownloadDefaultData_Progress, this, &UpdateUIController::OnDownloadDefaultDataProgress);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, LogicUpdateScene::m_DownloadSuitDetail_Progress, this, &UpdateUIController::OnDownloadSuitDetailProgress);
}

void UpdateUIController::OnRelease()
{
	m_HotUpdateUMG->OnRelease();
	m_HotUpdateUMG = nullptr;

	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LogicUpdateScene::m_DownloadPak_Progress, this, &UpdateUIController::OnDownloadPakProgress);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LogicUpdateScene::m_DownloadDefaultData_Progress, this, &UpdateUIController::OnDownloadDefaultDataProgress);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, LogicUpdateScene::m_DownloadSuitDetail_Progress, this, &UpdateUIController::OnDownloadSuitDetailProgress);


}

void UpdateUIController::OnBackstageUI()
{
	//m_HotUpdateUMG->OnActivationUI();
	//m_rootView->OnBackstageUI();
}

void UpdateUIController::OnActivationUI()
{

}

void UpdateUIController::ShowProgressUI()
{
	GetRootUI()->SetVisibility(ESlateVisibility::Hidden);
	m_HotUpdateUMG->OnActivationUI();
}

void UpdateUIController::OnDownloadPakProgress(msg_ptr _msg)
{

#if PLATFORM_WINDOWS
	((UWin_HotUpdateUMG*)m_HotUpdateUMG)->Reset();
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();
	DownlaodInfoStruct *mDownlaodInfoStruct = (DownlaodInfoStruct*)(mMsgLocal->Content());
	((UWin_HotUpdateUMG*)m_HotUpdateUMG)->SetProgress(mDownlaodInfoStruct);

#else
	((UHotUpdateUMG*)m_HotUpdateUMG)->Reset();
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();
	DownlaodInfoStruct *mDownlaodInfoStruct = (DownlaodInfoStruct*)(mMsgLocal->Content());
	((UHotUpdateUMG*)m_HotUpdateUMG)->SetProgress(mDownlaodInfoStruct);
#endif
}

void UpdateUIController::OnDownloadDefaultDataProgress(msg_ptr _msg)
{
#if PLATFORM_WINDOWS
	((UWin_HotUpdateUMG*)m_HotUpdateUMG)->Reset();
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();
	DownlaodInfoStruct *mDownlaodInfoStruct = (DownlaodInfoStruct*)(mMsgLocal->Content());
	((UWin_HotUpdateUMG*)m_HotUpdateUMG)->SetProgress(mDownlaodInfoStruct);
#else
	((UHotUpdateUMG*)m_HotUpdateUMG)->Reset();
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();
	DownlaodInfoStruct *mDownlaodInfoStruct = (DownlaodInfoStruct*)(mMsgLocal->Content());

	((UHotUpdateUMG*)m_HotUpdateUMG)->SetProgress(mDownlaodInfoStruct);
#endif
	
}

void UpdateUIController::OnDownloadSuitDetailProgress(msg_ptr _msg)
{
#if PLATFORM_WINDOWS
	((UWin_HotUpdateUMG*)m_HotUpdateUMG)->Reset();
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();
	DownlaodInfoStruct *mDownlaodInfoStruct = (DownlaodInfoStruct*)(mMsgLocal->Content());
	((UWin_HotUpdateUMG*)m_HotUpdateUMG)->SetProgress(mDownlaodInfoStruct);
#else
	((UHotUpdateUMG*)m_HotUpdateUMG)->Reset();
	MsgLocal * mMsgLocal = _msg->ConvertMsg<MsgLocal>();
	DownlaodInfoStruct *mDownlaodInfoStruct = (DownlaodInfoStruct*)(mMsgLocal->Content());
	((UHotUpdateUMG*)m_HotUpdateUMG)->SetProgress(mDownlaodInfoStruct);
#endif

}

void UpdateUIController::DownloadDefaultData()
{
	ShowProgressUI();
	LoadDataModule  *mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");
	mLoadDataModule->DownloadDefaultData();
}

void UpdateUIController::DownloadSuitDetail(int32 _suitTid)
{
	ShowProgressUI();
	LoadDataModule  *mLoadDataModule = GModuleManager::GetInstance()->GetModule<LoadDataModule>("LoadDataModule");
	mLoadDataModule->DownloadSuitDetailData(_suitTid);
}

void UpdateUIController::DownloadPak()
{
	ShowProgressUI();
	UHotLoadManager::GetInstance()->GetVersionXML();
}

