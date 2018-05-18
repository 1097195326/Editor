// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "BaseAlertUI.h"
#include "UIManager.h"
#include "GTimerMgr.h"


UBaseAlertUI::UBaseAlertUI(const FObjectInitializer& ObjectInitializer)
{

}

void UBaseAlertUI::NativeDestruct()
{
	StopTask();

	OnRelease();
}

void UBaseAlertUI::ClickBtnConfirm()
{
	m_alertDelegate->ButtonConfirmEvent();

	UUIManager::GetInstance()->CloseAlertByBaseAlertUI(this);
}

void UBaseAlertUI::ClickBtnCancel()
{
	m_alertDelegate->ButtonCancelEvent();

	UUIManager::GetInstance()->RemoveAllAlertUI();
}

void UBaseAlertUI::ClickBtnClose()
{
	UUIManager::GetInstance()->RemoveAllAlertUI();
}

void UBaseAlertUI::OnInitAlertUI()
{

}

void UBaseAlertUI::InitAlertUI(float _second, const FString & _content, UIAlertDelegate * _alertDelegate)
{
	m_second = _second;

	m_content = _content;

	if(nullptr != _alertDelegate)
		m_alertDelegate = _alertDelegate;

	// 开始倒计时.
	if (m_updateID != 0)
	{
		GTimerMgr::GetInstance()->RemoveTimer(m_updateID);

		m_updateID = 0;
	}
	if (m_second > 0.0f)
	{
		timer_task_ptr task(new TimerTask(this, &UBaseAlertUI::OnFinishTask, m_second, false));

		m_updateID = GTimerMgr::GetInstance()->RegisterTimer(task);
	}

	OnInitAlertUI();
}

bool UBaseAlertUI::OnFinishTask(float _delateTime)
{
	m_updateID = 0;

	UUIManager::GetInstance()->CloseAlertByBaseAlertUI(this);

	return true;
}

void UBaseAlertUI::StopTask()
{
	if (0 != m_updateID)
	{
		GTimerMgr::GetInstance()->RemoveTimer(m_updateID);

		m_updateID = 0;
	}
}
