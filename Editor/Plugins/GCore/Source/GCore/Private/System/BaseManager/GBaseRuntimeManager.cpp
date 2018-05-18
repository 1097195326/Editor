// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "GBaseRuntimeManager.h"
#include "GameDirector.h"

LoadUserStyleData::LoadUserStyleData()
{

}


GBaseRuntimeManager::GBaseRuntimeManager()
{

}

GBaseRuntimeManager::~GBaseRuntimeManager()
{

}


void GBaseRuntimeManager::GLoad()
{
	// 注册到GameDirector.自动释放内存.
	UGameDirector::GetInstance()->RegisterInstanceClass(this);

	OnLoad();
}

void GBaseRuntimeManager::GRelease()
{

	for (std::map<int32, BaseR*>::reverse_iterator rit = m_runtimeMap.rbegin(); rit != m_runtimeMap.rend(); rit++)
	{
		BaseR* mRuntime = rit->second;

		mRuntime->GRelease();
	}

	m_runtimeMap.clear();

	OnRelease();

	delete this;
}

void GBaseRuntimeManager::ClearBind()
{
	m_loadErrorDelegate = nullptr;

	m_loadFinishDelegate = nullptr;
}

BaseR* GBaseRuntimeManager::GetRuntimeByPID(int32 _pid)
{
	XY_IF_FOUND_IN_MAP(_pid, it, m_runtimeMap, RuntimePIDMap)
	{
		BaseR* mPrototype = it->second;

		return mPrototype;
	}
	return nullptr;
}

void GBaseRuntimeManager::ReleaseAllData()
{
	for (std::map<int32, BaseR*>::reverse_iterator rit = m_runtimeMap.rbegin(); rit != m_runtimeMap.rend(); rit++)
	{
		BaseR* mRuntime = rit->second;

		mRuntime->GRelease();
	}

	m_runtimeMap.clear();
}

void GBaseRuntimeManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{

}

void GBaseRuntimeManager::AddData(BaseR * _rData)
{
	XY_IF_FOUND_IN_MAP(_rData->GetRid(), it, m_runtimeMap, RuntimePIDMap)
	{
		BaseR * mRuntimData = it->second;

		m_runtimeMap.erase(it);

		mRuntimData->GRelease();
	}
	
	m_runtimeMap.insert(std::make_pair(_rData->GetRid(),_rData));
}

