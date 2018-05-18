// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "SchedulerCondition.h"
#include "castle_std.h"
/**
 * 
 */
class GCORE_API GSchedulerManager:public ILoad
{
public:
	GSchedulerManager();

	virtual ~GSchedulerManager();

	// 单利. 
	static GSchedulerManager * GetInstance();

private:

	static GSchedulerManager *           m_pInstance;

	// prototype管理的字典.
	TArray<SchedulerCondition*>			 m_schedulerArray;

	int32								 m_updateID = 0;

	TMap<FString, FString>               m_conditionMap;

public:

	virtual void GLoad() final;

	virtual void GRelease() final;

public:
	bool OnCheckSchedulerCondition(float _deltaTime);

	void AddSchedulerInfo(const FString & _schedulerInfo);

	void AddCondition(const FString & _key, const FString & _value);
};
