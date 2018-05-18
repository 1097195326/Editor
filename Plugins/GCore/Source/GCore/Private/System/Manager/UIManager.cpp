// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "UIManager.h"
#include "GController.h"
#include "GTimerMgr.h"
#include "UIPrototype.h"
#include "GameDirector.h"
#include "PrototypeManager.h"


UUIManager* UUIManager::m_pInstance = nullptr;//初始化在主线程之前.

											  // 单利.
UUIManager * UUIManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = NewObject<UUIManager>();

		m_pInstance->AddToRoot();
	}
	return m_pInstance;
}


UUIManager::UUIManager()
{
}

UUIManager::~UUIManager()
{    
}


void UUIManager::LoadPrototype(PlatformUIPrototype * _platformUIPrototype)
{
	m_platformUIPrototype = _platformUIPrototype;
}


void UUIManager::GLoad()
{

}

void UUIManager::OnUpdate(float DeltaTime)
{

}

UBaseAlertUI * UUIManager::GetAlertUI(const FString & _mUIPath)
{
	if (m_alertUIArray.Contains(_mUIPath))
	{
		return m_alertUIArray[_mUIPath];
	}
	return nullptr;
}

void UUIManager::RemoveAllButtonClick()
{
	if (!m_btnList.empty())
	{
		auto iter = m_btnList.begin();

		while (iter != m_btnList.end())
		{
			UButtonScript * mBtnScript = *iter;

			iter = m_btnList.erase(iter);

			mBtnScript->GRelease();
		}
	}
}

// 根据配置文件打开UI.
UBaseUI* UUIManager::OpenUI(const FString & _uiName)
{
	// 通过uiName找出UIPath.
	FString mUIPath = m_platformUIPrototype->GetUMGPath(_uiName);

	if (!mUIPath.IsEmpty())
	{
		UClass* loadClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, *mUIPath);

		if (loadClass == nullptr)
			return nullptr;
		  
		UWorld * mWorld = UGameDirector::GetInstance()->GetWorld();

		UBaseUI* _sceneObj = (UBaseUI*)CreateWidget<UUserWidget>(mWorld, loadClass);

		_sceneObj->GLoad();

		return _sceneObj;
	}
	return nullptr;
}

// 根据配置文件打开UI.
UBaseUI* UUIManager::OpenParentUI(const FString & _uiName)
{
	// 通过uiName找出UIPath.
	FString mParentUIName = m_platformUIPrototype->GetParentUIName(_uiName);

	FString mUIPath = m_platformUIPrototype->GetUMGPath(mParentUIName);

	return OpenUI(mUIPath);
}

void UUIManager::OpenTopTip(const FString & _content, float _second)
{
	FString mUIPath = TEXT("UI_Confirm");

	UBaseAlertUI * mAlertUI = GetAlertUI(mUIPath);

	UIAlertDelegate * mAlertDelegate = nullptr;

	if (nullptr == mAlertUI)
	{
		mAlertUI = (UBaseAlertUI *)OpenUI(mUIPath);

		mAlertDelegate = new UIAlertDelegate();

		m_alertUIArray.Add(mUIPath, mAlertUI);

		mAlertUI->AddToViewport();
	}

	mAlertUI->InitAlertUI(_second, _content, mAlertDelegate);
}

void UUIManager::OpenMiddleTip(const FString & _content, float _second)
{
	FString mUIPath = TEXT("UI_MsgInfoCenter");

	UBaseAlertUI * mAlertUI = GetAlertUI(mUIPath);

	UIAlertDelegate * mAlertDelegate = nullptr;

	if (nullptr == mAlertUI)
	{
		mAlertUI = (UBaseAlertUI *)OpenUI(mUIPath);

		mAlertDelegate = new UIAlertDelegate();

		mAlertUI->AddToViewport();

		m_alertUIArray.Add(mUIPath, mAlertUI);
	}

	mAlertUI->InitAlertUI(_second, _content, mAlertDelegate);
}

void UUIManager::OpenMiddleTip2(const FString & _content, float _second)
{
	FString mUIPath = TEXT("UI_MsgInfoCenter2");

	UBaseAlertUI * mAlertUI = GetAlertUI(mUIPath);

	UIAlertDelegate * mAlertDelegate = nullptr;

	if (nullptr == mAlertUI)
	{
		mAlertUI = (UBaseAlertUI *)OpenUI(mUIPath);

		mAlertDelegate = new UIAlertDelegate();

		mAlertUI->AddToViewport();

		m_alertUIArray.Add(mUIPath, mAlertUI);
	}

	mAlertUI->InitAlertUI(_second, _content, mAlertDelegate);
}

void UUIManager::OpenFullTip(const FString & _content)
{
	FString mUIPath = TEXT("UI_Waiting");

	UBaseAlertUI * mAlertUI = GetAlertUI(mUIPath);

	UIAlertDelegate * mAlertDelegate = nullptr;

	if (nullptr == mAlertUI)
	{
		mAlertUI = (UBaseAlertUI *)OpenUI(mUIPath);

		mAlertDelegate = new UIAlertDelegate();

		mAlertUI->AddToViewport();

		m_alertUIArray.Add(mUIPath, mAlertUI);
	}

	mAlertUI->InitAlertUI(0.0f, _content, mAlertDelegate);
}

void UUIManager::CloaseAlertByUMGPath(const FString & _uiPath)
{
	UBaseAlertUI * mAlertUI = GetAlertUI(_uiPath);

	if (nullptr != mAlertUI)
		mAlertUI->RemoveFromViewport();

	m_alertUIArray.Remove(_uiPath);
}

void UUIManager::CloseTopTip()
{
	FString mUIPath = TEXT("UI_Confirm");

	CloaseAlertByUMGPath(mUIPath);
}

void UUIManager::CloseMiddleTip()
{
	FString mUIPath = TEXT("UI_MsgInfoCenter");

	CloaseAlertByUMGPath(mUIPath);
}

void UUIManager::CloseTrueOrFalseAlert()
{
	FString mUIPath = TEXT("UI_ConfirmCancel");

	CloaseAlertByUMGPath(mUIPath);
}

void UUIManager::CloseTrueAlert()
{
	FString mUIPath = TEXT("UI_Confirm");

	CloaseAlertByUMGPath(mUIPath);
}

void UUIManager::CloseFullTip()
{
	FString mUIPath = TEXT("UI_Waiting");

	CloaseAlertByUMGPath(mUIPath);
}

void UUIManager::CloseAlertByBaseAlertUI(UBaseAlertUI * _alertUI)
{
	for (auto Iter = m_alertUIArray.CreateIterator(); Iter; ++Iter)
	{
		UBaseAlertUI * mAlertUI = Iter->Value;

		if (mAlertUI == _alertUI)
		{
			Iter.RemoveCurrent();

			// 删除掉.
			mAlertUI->RemoveFromViewport();
		}
	}
}

void UUIManager::RemoveAllAlertUI()
{
	// 每针检查一下哪些tip要删掉.
	for (auto Iter = m_alertUIArray.CreateIterator(); Iter; ++Iter)
	{
		UBaseAlertUI * mAlertUI = Iter->Value;

		Iter.RemoveCurrent();

		// 删除掉.
		if (mAlertUI && !mAlertUI->m_isDestroyed && mAlertUI->IsInViewport())
		{
			mAlertUI->RemoveFromViewport();
		}
	}
	
	m_alertUIArray.Empty();
}

void UUIManager::RegisterButtonClick(int32 _pid, UButton* _btn)
{
	UButtonScript * mBtnSript = UButtonScript::Create();

	mBtnSript->BindBtnEvent(BtnEnum::CLICKED, _pid, _btn);

	m_btnList.push_back(mBtnSript);
}
  
void UUIManager::RemoveButtonClick(UButton* _btn)
{
	auto iter = m_btnList.begin();

	while (iter != m_btnList.end())
	{
		UButtonScript * mBtnScript = *iter;

		if (_btn == mBtnScript->m_btn)
		{
			iter = m_btnList.erase(iter);

			mBtnScript->GRelease();
		}
		else
		{
			iter++;
		}
	}
}


void UUIManager::GRelease()
{
	//GTimerMgr::GetInstance()->RemoveGameUpdateHandler(this);

	if (!m_btnList.empty())
	{
		auto iter = m_btnList.begin();

		while (iter != m_btnList.end())
		{
			UButtonScript * mBtnScript = *iter;

			iter = m_btnList.erase(iter);

			mBtnScript->GRelease();
		}
		m_btnList.clear();
	}

	RemoveAllAlertUI();
	
	UUIManager::m_pInstance = nullptr;

	this->RemoveFromRoot();
}

