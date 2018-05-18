// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 状态.
 */
class GCORE_API IState
{
public:
	
	IState();

	virtual ~IState();

public:

	// 释放.
	virtual void GRelease() final;

	// 执行状态.
	virtual void Excute() = 0;

	// 停止状态.
	virtual void Stop() = 0;

	virtual void OnRelease() = 0;
};
