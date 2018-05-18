// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "BaseScheduler.h"
/**
 * 
 */
class GCORE_API SchedulerCondition:public ILoad
{
public:
	SchedulerCondition();

	virtual ~SchedulerCondition();

private:
	FString m_schedulerParam;

	// 协议方式.
	FString m_protocol;

	// 协议名称.
	FString m_protocolName;

	// 协议参数.
	TMap<FString, FString> m_protocolParam;

	// condition.
	TMap<FString,FString> m_protocolCondition;

	BaseScheduler * m_currentScheduler = nullptr;

public:
	virtual void GLoad() final;

	virtual void GRelease() final;

public:
	void InitScheduler(const FString & _schedulerParam);

	bool CheckCondition(TMap<FString,FString> & _conditionMap);

	void Excute();
};
