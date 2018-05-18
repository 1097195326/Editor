// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "ILoad.h"
#include "AARefClasses.h"
#include "UIController.h"
//#include "GResourceManager.h"
#include "GBaseGameMode.h"
#include "GObject.h"

/*
 * 逻辑场景类.
 */
class GCORE_API BaseLogicScene :public GObject, public ILoad
{
public:
	FString m_currentLevelName = TEXT("");

	FString m_currentLogicName = TEXT("");

public:
	BaseLogicScene();

	virtual ~BaseLogicScene();

private:
	// UI管理器,隐藏UI的方式压盏.
	TArray<UIController*> m_controllerMgr;

	AGBaseGameMode * m_gameMode = nullptr;

protected:
	//UGResourceManager * m_resourceManager = nullptr;

public:

	virtual void GLoad() final;

	virtual void GRelease() final;

protected:

	virtual void OnLoad();

	virtual void OnRelease();

protected:

	virtual void CloseCurrentController() final;


	// 打开UI的方法.
	virtual UBaseUI* CreateUI(const FString & _uiName) final;

public:

	// UI管理相关的方法,把其他UI都关闭掉.
	virtual UIController* OpenUIController(const FString & _controllerName) final;

	// 压盏式打开UI,隐藏之前的UI.
	virtual UIController* PushUIController(const FString & _controllerName) final;

	// 关闭当前UI，返回上一级UI.
	virtual void PopUIController() final;

	// 关闭所有的UI.
	virtual void CloseAllUIController() final;

	// 获取当前UI对象.
	virtual UIController * GetCurrentUIController() final;

protected:

	// 以下事件子类可以重写.
	virtual void OnOpenUIController(const FString & _controllerName);

	virtual void OnPushUIController(const FString & _controllerName);

	virtual void OnPopUIController();

public:
	// 当场景进入后台模式.
	virtual void BackstageLevel(const FString & _levelName) final;

	// 设置GameMode.
	virtual void SetGameMode(AGBaseGameMode * _gameMode, const FString & _levelName = TEXT("none")) final;


protected:

	virtual void OnBackstageLevel(const FString & _levelName);

	virtual void OnLevelBeginPlay(const FString & _levelName);
};
