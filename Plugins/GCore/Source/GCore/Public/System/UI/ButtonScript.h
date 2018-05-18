// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "BaseUI.h"
#include "castle_std.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <functional>
#include "ILoad.h"
#include "ButtonScript.generated.h"

/**
 * 
 */

enum BtnEnum
{
	CLICKED = 0,
	HOVER,
};

UCLASS()
class GCORE_API UButtonScript : public UUserWidget, public ILoad
{
	GENERATED_BODY()
	
public:
	UButton * m_btn;

	int32     m_pid=0;

	// 事件回调地址.
	std::function<void(int32)>          button_event_function;

public:
	// 单利. 
	static UButtonScript * Create();

	virtual ~UButtonScript();

	virtual void GLoad() override;

	virtual void GRelease() override;

	// 按钮点击事件.
	UFUNCTION()
	void ButtonClicked();

	// 绑定事件. 
	void BindBtnEvent(BtnEnum _btnEnum, int32 _pid, UButton * _btn);

	template<typename T>
	void BindBtnEvent(BtnEnum _btnEnum, int32 _pid, UButton * _btn, T* obj, void (T::*_func)(int32))
	{
		m_pid = _pid;

		m_btn = _btn;

		button_event_function = std::function<void(int32)>(std::bind(_func, obj, std::placeholders::_1));

		switch (_btnEnum) {

		case BtnEnum::CLICKED:
		{
			m_btn->OnClicked.AddDynamic(this, &UButtonScript::ButtonClicked);
			break;
		}

		case BtnEnum::HOVER:
		{
			m_btn->OnClicked.AddDynamic(this, &UButtonScript::ButtonClicked);
			break;
		}

		default:
			break;
		}
	}

};
