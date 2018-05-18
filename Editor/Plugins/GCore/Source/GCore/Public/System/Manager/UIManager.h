// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "BaseUI.h"
#include "BaseAlertUI.h"
#include "ButtonScript.h"
#include "UIAlertDelegate.h"
#include "PlatformUIPrototype.h"
#include "UObject/NoExportTypes.h"
#include "IUpdate.h"
#include "UIManager.generated.h"


/**
 * 
 */
UCLASS()
class GCORE_API UUIManager : public UObject,public ILoad,public IUpdate
{
	GENERATED_BODY()
	
private:

	// 按钮绑定事件注册.
	std::vector<UButtonScript*>        m_btnList;
	
	// 单利.
	static UUIManager *				   m_pInstance;

	// 当前显示的ui.
	UBaseUI*						   m_currentUI;

	PlatformUIPrototype *			   m_platformUIPrototype;

	// alert弹窗管理.
	TMap<FString, UBaseAlertUI*>       m_alertUIArray;


public:
	UUIManager();

	~UUIManager();

	static UUIManager * GetInstance();

	void LoadPrototype(PlatformUIPrototype * _platformUIPrototype);

	virtual void GLoad() override;

	virtual void GRelease() override;

	virtual void OnUpdate(float DeltaTime) override;

public:
	// 注册点击事件.
	void RegisterButtonClick(int32 _pid, UButton* _btn);

	template<typename T>
	void RegisterButtonClick(int32 _pid, UButton* _btn, T* obj, void (T::*_func)(int32 _pid))
	{
		UButtonScript * mBtnSript = UButtonScript::Create();

		mBtnSript->BindBtnEvent(BtnEnum::CLICKED, _pid, _btn, obj, _func);

		m_btnList.push_back(mBtnSript);
	}

	// 移除按钮点击事件.
	void RemoveButtonClick(UButton* _btn);

	// 移除按钮点击事件.
	void RemoveAllButtonClick();

private:
	void CloaseAlertByUMGPath(const FString & _uiPath);

public:

	// 关闭所有的alert.
	void RemoveAllAlertUI();

	UBaseAlertUI * GetAlertUI(const FString & _mUIPath);

	// 普通UI打开方法.
	UBaseUI * OpenUI(const FString & _uiName);

	// 打开父级UI的方法.
	UBaseUI * OpenParentUI(const FString & _uiName);

	// 打开上面提示的Tip显示秒.
	void OpenTopTip(const FString & _content, float _second);

	// 打开中间提示的tip.
	void OpenMiddleTip(const FString & _content, float _second);

	// 打开中间提示的tip.
	void OpenMiddleTip2(const FString & _content, float _second);

	// 打开全屏的tip.
	void OpenFullTip(const FString & _content);

	void CloseTopTip();
	void CloseMiddleTip();
	void CloseTrueOrFalseAlert();
	void CloseTrueAlert();
	void CloseFullTip();

	// 通过ui对象关闭ui.
	void CloseAlertByBaseAlertUI(UBaseAlertUI * _alertUI);

	// 打开确认跟取消按钮的弹窗.
	template<typename T>
	void OpenTrueOrFalseAlert(const FString & _content,T* obj, void (T::*_button_confirm_event)(),void (T::*_button_cancel_event)())
	{
		FString mUIPath = TEXT("UI_ConfirmCancel");

		UBaseAlertUI * mAlertUI = GetAlertUI(mUIPath);

		UIAlertDelegate * mAlertDelegate = nullptr;

		if (nullptr == mAlertUI)
		{
			mAlertUI = (UBaseAlertUI *)OpenUI(mUIPath);

			mAlertDelegate = new UIAlertDelegate();

			mAlertDelegate->InitCancelEvent(obj, _button_cancel_event);
			mAlertDelegate->InitConfirmEvent(obj, _button_confirm_event);

			m_alertUIArray.Add(mUIPath, mAlertUI);

			mAlertUI->AddToViewport();
		}

		mAlertUI->InitAlertUI(0.0f, _content, mAlertDelegate);

	}

	// 打开只有确定按钮的弹出框.
	template<typename T>
	void OpenTrueAlert(const FString & _content, T* obj, void (T::*_button_confirm_event)())
	{
		FString mUIPath = TEXT("UI_Confirm");

		UBaseAlertUI * mAlertUI = GetAlertUI(mUIPath);

		UIAlertDelegate * mAlertDelegate = nullptr;

		if (nullptr == mAlertUI)
		{
			mAlertUI = (UBaseAlertUI *)OpenUI(mUIPath);

			mAlertDelegate = new UIAlertDelegate();

			mAlertDelegate->InitConfirmEvent(obj, _button_confirm_event);

			m_alertUIArray.Add(mUIPath, mAlertUI);

			mAlertUI->AddToViewport();
		}

		mAlertUI->InitAlertUI(0.0f, _content, mAlertDelegate);
	}
};
