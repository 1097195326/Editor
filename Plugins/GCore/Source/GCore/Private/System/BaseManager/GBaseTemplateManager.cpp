// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "GBaseTemplateManager.h"
#include "GameDirector.h"


GBaseTemplateManager::GBaseTemplateManager()
{

}

GBaseTemplateManager::~GBaseTemplateManager()
{

}



void GBaseTemplateManager::GLoad()
{
	// 注册到GameDirector.自动释放内存.
	UGameDirector::GetInstance()->RegisterInstanceClass(this);

	OnLoad();
}

void GBaseTemplateManager::GRelease()
{
	for (std::map<int32, BaseT*>::reverse_iterator rit = m_templateMap.rbegin(); rit != m_templateMap.rend(); rit++)
	{
		BaseT* mTemplate = rit->second;

		mTemplate->GRelease();
	}

	m_templateMap.clear();

	OnRelease();

	delete this;
}

BaseT* GBaseTemplateManager::GetTemplateByPID(int32 _pid)
{
	XY_IF_FOUND_IN_MAP(_pid, it, m_templateMap, TeamplatePIDMap)
	{
		BaseT* mPrototype = it->second;

		return mPrototype;
	}
	return nullptr;
}

void GBaseTemplateManager::ReleaseAllData()
{
	for (std::map<int32, BaseT*>::reverse_iterator rit = m_templateMap.rbegin(); rit != m_templateMap.rend(); rit++)
	{
		BaseT* mTemplate = rit->second;

		mTemplate->GRelease();
	}

	m_templateMap.clear();
}

void GBaseTemplateManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{

}

void GBaseTemplateManager::AddData(BaseT * _content)
{
	XY_IF_FOUND_IN_MAP(_content->GetTid(), it, m_templateMap, TeamplatePIDMap)
	{
		BaseT * mData = it->second;

		m_templateMap.erase(it);

		mData->GRelease();
	}

	m_templateMap.insert(std::make_pair(_content->GetTid(), _content));
}
