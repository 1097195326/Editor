// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"  
#include "Runtime/UMG/Public/UMGStyle.h"  
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"  
#include "ILoad.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "BaseUI.generated.h"

/**
 * 
 */
UCLASS()
class GCORE_API UBaseUI : public UUserWidget,public ILoad
{
	GENERATED_UCLASS_BODY()
	
public:
	bool m_isDestroyed = false;

	virtual ~UBaseUI();
	
public: // 这个区间的类不可以重写.

	// 初始化UI事件.
	virtual bool Initialize() override;

	// 创建UI事件.
	virtual void NativeConstruct() override;

	// 释放UI事件.
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 当场景进入后台模式.
	virtual void BackstageUI();

	// 当场景从后台被激活的时间.
	virtual void ActivationUI();

	virtual void GLoad() override;

	virtual void GRelease() override;

public: // 重写以下的类.
	
	virtual bool OnStart();                           
	 
	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnLoad();

	// 被传入参数的列表.
	virtual void OnSetParams(int32 _pid,FString & _content,...);

	// update.
	virtual void OnTick(float InDeltaTimeSecond);

public: // 子类需要重写.

	virtual void OnBackstageUI();

	virtual void OnActivationUI();
	
};
