// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <functional>
#include "BaseUI.h"

/**
 * 当前tip打开的UI 事件管理等.
 */
class GCORE_API UIAlertDelegate
{
private:
	
	// 点击确定时候的函数回调事件.
	std::function<void()>          m_button_confirm_event = nullptr;

	// 点击取消按钮回调事件.
	std::function<void()>          m_button_cancel_event = nullptr;

public:

	UIAlertDelegate();
	~UIAlertDelegate();

	template<typename T>
	void InitConfirmEvent(T* obj, void (T::*_button_confirm_event)())
	{
		if(nullptr != _button_confirm_event)
			m_button_confirm_event = std::function<void()>(std::bind(_button_confirm_event, obj));
	}

	template<typename T>
	void InitCancelEvent(T* obj, void (T::*_button_cancel_event)())
	{
		if (nullptr != _button_cancel_event)
			m_button_cancel_event = std::function<void()>(std::bind(_button_cancel_event, obj));
	}

public:
	void ButtonConfirmEvent();

	void ButtonCancelEvent();

};
