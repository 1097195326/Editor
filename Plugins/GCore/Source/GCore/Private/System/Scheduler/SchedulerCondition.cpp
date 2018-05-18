// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "PrototypeManager.h"
#include "SchedulerMapping.h"
#include "SchedulerCondition.h"

SchedulerCondition::SchedulerCondition()
{
}

SchedulerCondition::~SchedulerCondition()
{
}

void SchedulerCondition::GLoad()
{

}

void SchedulerCondition::GRelease()
{
	delete this;
}

void SchedulerCondition::InitScheduler(const FString & _schedulerParam)
{
	// 解析参数.
	FString mParam;

	_schedulerParam.Split(TEXT("/"), &m_schedulerParam, &mParam);

	// 解析参数.
	if (!mParam.IsEmpty())
	{
		TArray<FString> mSplitParamArray;

		mParam.ParseIntoArray(mSplitParamArray, TEXT(","));

		for (int32 i = 0; i < mSplitParamArray.Num(); i++)
		{
			FString mLeft;

			FString mRight;

			mSplitParamArray[i].Split(TEXT("="), &mLeft, &mRight);

			m_protocolParam.Add(mLeft, mRight);
		}
	}

	// 协议方式.
	TArray<FString> mSplitArray;

	int32 mSplitCount = m_schedulerParam.ParseIntoArray(mSplitArray, TEXT("-"));

	// h5-OpenH5Scheduler-login=1,param=http://www.baidu.com.
	if (mSplitCount > 0)
	{
		m_protocol = mSplitArray[0];

		// 协议名称.
		if (mSplitCount > 1)
		{
			m_protocolName = mSplitArray[1];

			// 反射出来一个schduler.
			SchedulerMapping * mSchedulerMapping = PrototypeManager::GetInstance()->GetPrototypeByPID<SchedulerMapping>(PrototypeConst::P_SCHEDULER_MAPPING_ID);
			
			std::string mRefName = TCHAR_TO_UTF8(*mSchedulerMapping->GetClassName(m_protocolName));

			m_currentScheduler = (BaseScheduler*)(AARefClasses::GetInstance()->CreateClassByName(mRefName));

			m_currentScheduler->GLoad();
		}

		// 协议condition.
		if (mSplitCount > 2)
		{
			FString mConditions = mSplitArray[2];

			TArray<FString> mSplitConditionArray;

			mConditions.ParseIntoArray(mSplitConditionArray, TEXT(","));

			for (int32 i=0;i<mSplitConditionArray.Num();i++)
			{
				FString mLeft;

				FString mRight;

				mSplitConditionArray[i].Split(TEXT("="), &mLeft, &mRight);

				m_protocolCondition.Add(mLeft, mRight);
			}
		}
	}
}

bool SchedulerCondition::CheckCondition(TMap<FString, FString> & _conditionMap)
{
	// 检查状态.
	bool isConditionSuccess = true;

	for (auto Iter = m_protocolCondition.CreateIterator(); Iter; ++Iter)
	{
		FString mKey = Iter->Key;

		FString mValue = Iter->Value;
		
		if (_conditionMap.Contains(mKey))
		{
			if (!_conditionMap[mKey].Equals(mValue))
			{
				isConditionSuccess = false;
			}
		}
		else
		{
			isConditionSuccess = false;
		}
	}
	return isConditionSuccess;
}

void SchedulerCondition::Excute()
{
	if(nullptr != m_currentScheduler)
		m_currentScheduler->Excute(m_protocolParam);
}
