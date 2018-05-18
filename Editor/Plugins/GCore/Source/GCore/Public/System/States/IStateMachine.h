// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 有限状态机.
 */
class GCORE_API IStateMachine
{
public:
	IStateMachine();

	virtual ~IStateMachine();

public:
	// 切换状态.
	virtual void ChangeState(int32 _state) = 0;

	virtual void GRelease() = 0;

	virtual int32 GetCurrentState() = 0;
};
