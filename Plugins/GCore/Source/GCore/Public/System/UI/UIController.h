// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "BaseUI.h"
#include "UITextureManager.h"
#include "UIManager.h"
#include "GCore/Public/AAReflection/AARefClasses.h"
/**
 * 
 */
class GCORE_API UIController:public ILoad
{

private:
	// root UI.
	UBaseUI * m_rootView = nullptr;

protected:
	UUITextureManager * m_textureManager = nullptr;

public:
	UIController();

	virtual ~UIController();

	virtual void GLoad() final;

	virtual void GRelease() final;

	UUITextureManager * GetTextureManager();

protected: // 以下类必须重写.

	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

	virtual void OnBackstageUI();

	virtual void OnActivationUI();

	virtual void SetRootView(UBaseUI * _rootView) final;


public: // 不可以重写.

		// 当场景进入后台模式.
	virtual void BackstageUI();

	// 当场景从后台被激活的时间.
	virtual void ActivationUI();

	UBaseUI * GetRootUI();

public: // 子类需要重写.


};
