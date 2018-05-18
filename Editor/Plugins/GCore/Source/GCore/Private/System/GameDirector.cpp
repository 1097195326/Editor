// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "GTimerMgr.h"
#include "GPlatformMgr.h"
#include "AARefClasses.h"
#include "GController.h"
#include "UIManager.h"
#include "GResourceManager.h"
#include "GSchedulerManager.h"
#include "DownloadImageTool.h"
#include "GameDirector.h"
#include "PrototypeManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "GameDirector.h"

UGameDirector * UGameDirector::m_instance = nullptr;

UGameDirector::UGameDirector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}

UGameDirector * UGameDirector::GetInstance()
{
	return m_instance;
}

// 初始化的方法.
void UGameDirector::Init()
{
	Super::Init();

	m_instance = this;

	if (!m_isInit)
	{
		m_isInit = true;

		GLoad();
	}
}


void UGameDirector::Shutdown()
{
	m_isInit = false;

	GRelease();
}

void UGameDirector::GLoad()
{
	UGResourceManager::GetInstance()->GLoad();

	GTimerMgr::GetInstance()->GLoad();

	GPlatformMgr::GetInstance()->GLoad();

	GSchedulerManager::GetInstance()->GLoad();

	OnLoad();

	if (GController::m_platform == ENUM_PLATFORM::GWINDOWS)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(100);
	}
	else if (GController::m_platform == ENUM_PLATFORM::GMAC)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(101);
	}
	else if (GController::m_platform == ENUM_PLATFORM::GANDROID)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(102);
	}
	else if (GController::m_platform == ENUM_PLATFORM::GIOS)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(103);
	}

	// 启动定时器.
	timer_task_ptr task(new TimerTask(this, &UGameDirector::OnUpdateReceived, 2.0f, true));

	m_updateID = GTimerMgr::GetInstance()->RegisterTimer(task);
}

bool UGameDirector::OnUpdateReceived(float DeltaTime)
{
	GWorld->GetWorld()->ForceGarbageCollection(true);
	return false;
}

void UGameDirector::GRelease()
{
	GTimerMgr::GetInstance()->RemoveTimer(m_updateID);

	GSchedulerManager::GetInstance()->GRelease();

	CloseAllLogicScene();

	UGResourceManager::GetInstance()->GRelease();

	// 先释放所有单利管理里的类. 
	for (int32 i = m_gameInstanceArr.Num() - 1; i >= 0; i--)
	{
		ILoad * task = m_gameInstanceArr[i];

		// 从数组中删除.
		m_gameInstanceArr.RemoveAt(i);

		task->GRelease();

		task = nullptr;
	}

	m_gameInstanceArr.RemoveAll([](ILoad* Val) {
		return true;
	});

	// 释放掉所有的反射管理.
	AARefClasses::GetInstance()->GRelease();

	m_gameInstanceArr.Empty();

	GTimerMgr::GetInstance()->GRelease();

	GPlatformMgr::GetInstance()->GRelease();

	OnRelease();

	UDownloadImageTool::GetInstance()->GRelease();
}

void UGameDirector::OnLoad()
{

}

void UGameDirector::OnRelease()
{

}

void UGameDirector::QuitGame()
{
#if PLATFORM_IOS
	exit(0);
#else  
	UWorld * mWorld = GetWorld();
	
	APlayerController * mPlayerCtr = mWorld->GetFirstPlayerController();

	if (mPlayerCtr)
	{
		UKismetSystemLibrary::QuitGame((UObject*)mWorld, mPlayerCtr, EQuitPreference::Quit);
	}
#endif
}

void UGameDirector::RegisterInstanceClass(ILoad * _class_s)
{
	m_gameInstanceArr.Push(_class_s);
}

FStreamableManager & UGameDirector::StreamableManager()
{
	return m_streamable;
}

void UGameDirector::OpenLevel(const FString & _levelName)
{
	UUIManager::GetInstance()->RemoveAllAlertUI();

	FString mScenePath = m_scenePrototype->GetLevelName(_levelName);

	UGameplayStatics::OpenLevel(UGameDirector::GetInstance(), *mScenePath);
}

BaseLogicScene * UGameDirector::GetCurrentLogicScene()
{
	if (m_sceneMgr.Num() > 0)
	{
		int32 i = m_sceneMgr.Num() - 1;

		return m_sceneMgr[i];
	}
	return nullptr;
}

void UGameDirector::CloseCurrentLogicScene()
{
	UUIManager::GetInstance()->RemoveAllAlertUI();
	if (m_sceneMgr.Num() > 0)
	{
		int32 i = m_sceneMgr.Num() - 1;

		BaseLogicScene * mScene = m_sceneMgr[i];

		// 从数组中删除.
		m_sceneMgr.RemoveAt(i);

		mScene->GRelease();
	}
}

void UGameDirector::CloseAllLogicScene()
{
	// 先释放所有单利管理里的类. 
	for (int32 i = m_sceneMgr.Num() - 1; i >= 0; i--)
	{
		BaseLogicScene * mScene = m_sceneMgr[i];

		// 从数组中删除.
		m_sceneMgr.RemoveAt(i);

		mScene->GRelease();
	}
}

void UGameDirector::LoadLevelAsync(const FString & _levelName)
{
	if (nullptr != m_scenePrototype)
	{
		FString mLanguage = GLanguageTool::GetInstance()->GetLanguage(TEXT("key133"));

		UUIManager::GetInstance()->OpenMiddleTip(mLanguage, 0.0f);

		FString mScenePath = m_scenePrototype->GetLevelName(_levelName);

		LoadPackageAsync(mScenePath, FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
		{
			UUIManager::GetInstance()->RemoveAllAlertUI();

			UGameplayStatics::OpenLevel(UGameDirector::GetInstance(), *mScenePath);
		}),
			0, EPackageFlags::PKG_ContainsMap);
	}
}

void UGameDirector::OpenLogicScene(const FString & _sceneName, AGBaseGameMode * _gameMode, const FString & _levelName)
{
	// 关闭所有其他的场景.
	BaseLogicScene * mLogicScene = GetCurrentLogicScene();
	
	if (nullptr != mLogicScene && mLogicScene->m_currentLogicName.Equals(_sceneName))
	{
		mLogicScene->SetGameMode(_gameMode, _levelName);
	}
	else
	{
		CloseAllLogicScene();

		mLogicScene = (BaseLogicScene*)AARefClasses::GetInstance()->CreateClassByName(TCHAR_TO_UTF8(*_sceneName));

		mLogicScene->m_currentLogicName = _sceneName;

		m_sceneMgr.Add(mLogicScene);

		mLogicScene->GLoad();

		if (nullptr != _gameMode)
			mLogicScene->SetGameMode(_gameMode, _levelName);
	}

}

void UGameDirector::ChangeLevel(AGBaseGameMode * _gameMode, const FString & _levelName /*= TEXT("")*/)
{
	// 获取当前UI.
	BaseLogicScene * mScene = GetCurrentLogicScene();

	if (nullptr != mScene)
	{
		mScene->SetGameMode(_gameMode, _levelName);
	}
}

void UGameDirector::OnEndPlayGameModeEvent(const FString & _levelName /*= TEXT("")*/)
{
	// 获取当前场景.
	BaseLogicScene * mScene = GetCurrentLogicScene();

	if (nullptr != mScene)
	{
		mScene->BackstageLevel(_levelName);
	}
}

void UGameDirector::ShowMouseCursor()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}
