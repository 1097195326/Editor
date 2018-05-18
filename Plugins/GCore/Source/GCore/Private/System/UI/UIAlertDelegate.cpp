// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "UIAlertDelegate.h"
#include "GTimerMgr.h"



UIAlertDelegate::UIAlertDelegate()
{

}

UIAlertDelegate::~UIAlertDelegate()
{

}


void UIAlertDelegate::ButtonConfirmEvent()
{
	if (m_button_confirm_event)
		m_button_confirm_event();
}

void UIAlertDelegate::ButtonCancelEvent()
{
	if (m_button_cancel_event)
		m_button_cancel_event();
}
