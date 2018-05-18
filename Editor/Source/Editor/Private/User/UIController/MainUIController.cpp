// Fill out your copyright notice in the Description page of Project Settings.

#include "MainUIController.h"
#include "MainUMG.h"
#include "SelectCell.h"
#include "Win_MainUMG.h"
#include "GModuleManager.h"
#include "EditorDirector.h"
#include "Win_SelectCell.h"
#include "Win_RecommendStyleUMG.h"

//	反射.
GREGISTER(MainUIController)

MainUIController::MainUIController()
{
	m_PreView = nullptr;
	m_CurrentView = nullptr;
}

void MainUIController::ChangePageTo(PAGE_TYPE _type)
{
	//	切换UI.
	switch (_type)
	{
	case PAGE_TYPE::MAIN_PAGE:
		//GetRootUI()->RemoveFromViewport();
		//GetRootUI()->SetVisibility(ESlateVisibility::Visible);
 		SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("MainUMG")));
		GetRootUI()->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT(" MainUIController::ChangePageTo"));
	
		break;

	case PAGE_TYPE::Sport_Page :
		UWin_RecommendStyleUMG::m_ParentId = 1025;
		URecommendStyleUMG::m_ParentId = 1025;
		m_RecommentStyle = UUIManager::GetInstance()->OpenUI(TEXT("RecommendStyle"));
		m_RecommentStyle->AddToViewport();
		break;

	case PAGE_TYPE::StyleSpecialShowList:
		break;

	case PAGE_TYPE::Recommend_PAGE:
		UWin_RecommendStyleUMG::m_ParentId = 1006;
		URecommendStyleUMG::m_ParentId = 1006;
		m_RecommentStyle = UUIManager::GetInstance()->OpenUI(TEXT("RecommendStyle"));
		m_RecommentStyle->AddToViewport();
		break;

	case PAGE_TYPE::MyWork_PAGE:
		m_MyWorks = UUIManager::GetInstance()->OpenUI(TEXT("UI_MyWorks"));
		m_MyWorks->AddToViewport();
		break;

	case PAGE_TYPE::PersonInfo_PAGE:
		GetRootUI()->RemoveFromViewport();
		m_PersonInfo = UUIManager::GetInstance()->OpenUI(TEXT("UI_PersonInfo"));
		m_PersonInfo->AddToViewport();
		//GetRootUI()->SetVisibility(ESlateVisibility::Hidden);
		break;

	case PAGE_TYPE::Photo_PAGE:
		m_PhotoPage = UUIManager::GetInstance()->OpenUI(TEXT("UI_Photo"));
		m_PhotoPage->AddToViewport();
		break;

	case PAGE_TYPE::DisplaySetting_PAGE:
		m_DisplaySetting = UUIManager::GetInstance()->OpenUI(TEXT("UI_DisplaySetting"));
		m_DisplaySetting->AddToViewport();
		break;

	case PAGE_TYPE::LanguageSetting_PAGE:
		m_LanguageSetting =UUIManager::GetInstance()->OpenUI(TEXT("UI_LanguageSetting"));
		m_LanguageSetting->AddToViewport();
		break;
	}
}

MainUIController::~MainUIController()
{

}

void MainUIController::OnLoad()
{
	// 初始化要打开的根UI.
	SetRootView(UUIManager::GetInstance()->OpenUI(TEXT("MainUMG")));
	
	GetRootUI()->AddToViewport();

	m_CurrentView = GetRootUI();

	//显示鼠标.
	UGameDirector::GetInstance()->ShowMouseCursor();
}

void MainUIController::OnRelease()
{
	// 需要把子view管理.rootView已经在父类释放掉了.

	MainModule *mMainModule = GModuleManager::GetInstance()->GetModule<MainModule>("MainModule");
}

void MainUIController::OnBackstageUI()
{

}

void MainUIController::OnActivationUI()
{
	
}