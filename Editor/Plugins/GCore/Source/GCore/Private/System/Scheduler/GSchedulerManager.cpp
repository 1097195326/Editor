// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "GSchedulerManager.h"
#include "GTimerMgr.h"

GSchedulerManager* GSchedulerManager::m_pInstance = nullptr;//初始化在主线程之前.

GSchedulerManager::GSchedulerManager()
{
}

GSchedulerManager::~GSchedulerManager()
{
}     

GSchedulerManager * GSchedulerManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new GSchedulerManager();
	}
	return m_pInstance;
}

void GSchedulerManager::GLoad()
{
	// 启动定时器.
	timer_task_ptr task(new TimerTask(this, &GSchedulerManager::OnCheckSchedulerCondition, 0.0f, true));

	m_updateID = GTimerMgr::GetInstance()->RegisterTimer(task);
}

bool GSchedulerManager::OnCheckSchedulerCondition(float _deltaTime)
{
	for (int32 i= m_schedulerArray.Num()-1;i>=0;i--)
	{
		SchedulerCondition* mSchedulerCondition = m_schedulerArray[i];

		if (mSchedulerCondition->CheckCondition(m_conditionMap))
		{
			mSchedulerCondition->Excute();

			m_schedulerArray.RemoveAt(i);

			mSchedulerCondition->GRelease();
		}
	}
	return false;
}

void GSchedulerManager::AddSchedulerInfo(const FString & _schedulerInfo)
{
	// 创建一个condition.
	SchedulerCondition * mSchedulerCondition = new SchedulerCondition();

	mSchedulerCondition->InitScheduler(_schedulerInfo);

	m_schedulerArray.Add(mSchedulerCondition);

	OnCheckSchedulerCondition(0.0f);
}

void GSchedulerManager::AddCondition(const FString & _key, const FString & _value)
{
	if (m_conditionMap.Contains(_key))
	{
		m_conditionMap[_key] = _value;
	}
	else
	{
		m_conditionMap.Add(_key, _value);
	}
}

void GSchedulerManager::GRelease()
{
	for (int32 i= m_schedulerArray.Num()-1;i>=0;i--)
	{
		SchedulerCondition* mSchedulerCondition = m_schedulerArray[i];

		mSchedulerCondition->GRelease();
	}

	m_schedulerArray.Empty();

	GTimerMgr::GetInstance()->RemoveTimer(m_updateID);

	GSchedulerManager::m_pInstance = nullptr;

	delete this;
}
