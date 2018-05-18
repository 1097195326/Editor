// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ILoad.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "BaseLogicScene.h"
#include "castle_std.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <functional>
#include "GLanguageTool.h"
#include "ScenePrototype.h"
#include "GameDirector.generated.h"

/**
 * 
 */
UCLASS()
class GCORE_API UGameDirector : public UGameInstance,public ILoad
{
	GENERATED_BODY()

private:
	static UGameDirector * m_instance;

	bool m_isInit=false;

	int32 m_updateID = 0;

	// 保存游戏中的所有单利类，最后释放掉.
	TArray<ILoad*>      m_gameInstanceArr;

	// 逻辑场景管理对象(栈队列管理,后进先出).
	TArray<BaseLogicScene*> m_sceneMgr;

	// 异步加载用的.
	FStreamableManager m_streamable;

	ScenePrototype * m_scenePrototype = nullptr;

	std::function<void(FString, EAsyncLoadingResult::Type)> m_onLoadSuccessLevelEvent;

public:

	UGameDirector(const FObjectInitializer & objectInitializer);

	static UGameDirector * GetInstance();
    
	virtual void Init() override;

	virtual void Shutdown() override;

	virtual void GLoad() override;

	virtual void GRelease() override;

public:

	// 子类需要重写.
	virtual void OnLoad();

	virtual void OnRelease();

	virtual void QuitGame();

public:// 以下方法不可以重写.
	
	// 保存游戏中的所有单利类，最后释放掉.
	void RegisterInstanceClass(ILoad * _class_s);
	
	FStreamableManager & StreamableManager();

public: // 场景管理相关的类,子类不要重写.

	// 打开渲染场景.
	void OpenLevel(const FString & _levelName);

	void LoadLevelAsync(const FString & _levelName);

	template<typename T>
	void LoadLevelAsync(const FString & _levelName, T* obj, void (T::*_func)(FString levelName, EAsyncLoadingResult::Type _result))
	{
		if (nullptr != m_scenePrototype)
		{
			m_onLoadSuccessLevelEvent = std::function<void(FString, EAsyncLoadingResult::Type)>(std::bind(_func, obj, std::placeholders::_1, std::placeholders::_2));

			FString mScenePath = m_scenePrototype->GetLevelName(_levelName);

			LoadPackageAsync(mScenePath, FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				m_onLoadSuccessLevelEvent(mScenePath, Result);
			}),
				0, EPackageFlags::PKG_ContainsMap);
		}
	}

	// 彻底切换场景,要是有上一个场景就直接卸载掉.
	void OpenLogicScene(const FString & _logicSceneName,AGBaseGameMode * _gameMode, const FString & _levelName = TEXT(""));

	// 切换render(gamemode)场景.
	void ChangeLevel(AGBaseGameMode * _gameMode,const FString & _levelName = TEXT(""));

	// 当前场景关闭状态.
	void OnEndPlayGameModeEvent(const FString & _levelName = TEXT(""));

	BaseLogicScene * GetCurrentLogicScene();

	void ShowMouseCursor();

public:
	void CloseCurrentLogicScene();

	void CloseAllLogicScene();

	bool OnUpdateReceived(float DeltaTime);

};
