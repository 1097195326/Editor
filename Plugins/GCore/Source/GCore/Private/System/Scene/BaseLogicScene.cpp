// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "GameDirector.h"
#include "UIManager.h"
#include "BaseLogicScene.h"

BaseLogicScene::BaseLogicScene()
{
}

BaseLogicScene::~BaseLogicScene()
{
}

void BaseLogicScene::GLoad()
{
	OnLoad();
}

void BaseLogicScene::GRelease()
{
	CloseAllUIController();

	OnRelease();

	delete this;
}

void BaseLogicScene::OnLoad()
{

}

void BaseLogicScene::OnRelease()
{

}

void BaseLogicScene::CloseCurrentController()
{
	if (m_controllerMgr.Num() > 0)
	{
		int32 i = m_controllerMgr.Num() - 1;

		UIController * mController = m_controllerMgr[i];

		// 从数组中删除.
		m_controllerMgr.RemoveAt(i);

		mController->GRelease();
	}
	m_controllerMgr.Empty();
}

UIController* BaseLogicScene::OpenUIController(const FString & _controllerName)
{
	CloseAllUIController();

	UIController * mController = (UIController*)AARefClasses::GetInstance()->CreateClassByName(TCHAR_TO_UTF8(*_controllerName));

	m_controllerMgr.Add(mController);

	mController->GLoad();

	OnOpenUIController(_controllerName);

	return mController;
}

UBaseUI* BaseLogicScene::CreateUI(const FString & _uiName)
{
	UBaseUI * mUI = UUIManager::GetInstance()->OpenUI(_uiName);

	return mUI;
}

UIController* BaseLogicScene::PushUIController(const FString & _controllerName)
{
	UIController * mController = (UIController*)AARefClasses::GetInstance()->CreateClassByName(TCHAR_TO_UTF8(*_controllerName));

	m_controllerMgr.Add(mController);

	mController->GLoad();

	OnPushUIController(_controllerName);

	return mController;
}

void BaseLogicScene::PopUIController()
{
	// 关闭当前UI.
	CloseCurrentController();

	// 打开最后一层的UI.

	UIController * mController = nullptr;

	if (m_controllerMgr.Num() > 0)
	{
		mController = m_controllerMgr[m_controllerMgr.Num() - 1];
	}

	if (nullptr != mController)
	{
		mController->ActivationUI();
	}

	OnPopUIController();
}

void BaseLogicScene::CloseAllUIController()
{
	// 先释放所有单利管理里的类. 
	for (int32 i = 0; i < m_controllerMgr.Num(); ++i)
	{
		UIController * mController = m_controllerMgr[i];

		mController->GRelease();
	}
	m_controllerMgr.Empty();
}

UIController * BaseLogicScene::GetCurrentUIController()
{
	if (m_controllerMgr.Num() > 0)
	{
		return m_controllerMgr[m_controllerMgr.Num() - 1];
	}
	return nullptr;
}

void BaseLogicScene::SetGameMode(AGBaseGameMode * _gameMode, const FString & _levelName /*=TEXT("none")*/)
{
	m_gameMode = _gameMode;

	m_currentLevelName = _levelName;

	OnLevelBeginPlay(_levelName);
}

void BaseLogicScene::OnOpenUIController(const FString & _uiName)
{

}

void BaseLogicScene::OnPushUIController(const FString & _uiName)
{

}

void BaseLogicScene::OnPopUIController()
{

}

void BaseLogicScene::BackstageLevel(const FString & _levelName)
{
	// 隐藏所有的UI.
	CloseAllUIController();

	m_gameMode = nullptr;

	OnBackstageLevel(_levelName);
}

void BaseLogicScene::OnBackstageLevel(const FString & _levelName)
{

}

void BaseLogicScene::OnLevelBeginPlay(const FString & _levelName)
{

}
