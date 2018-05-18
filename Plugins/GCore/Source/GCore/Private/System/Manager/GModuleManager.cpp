// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "GModuleManager.h"

GModuleManager* GModuleManager::m_pInstance = nullptr;//初始化在主线程之前.

											// 单利.
GModuleManager * GModuleManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new GModuleManager();
	}
	return m_pInstance;
}


GModuleManager::GModuleManager()
{

}

GModuleManager::~GModuleManager()
{
}


void GModuleManager::GLoad()
{

}

void GModuleManager::GRelease()
{
	GModuleManager::m_pInstance = nullptr;

	// 把所有剩下的module都release掉.
	for (std::map<std::string, BaseModule*>::reverse_iterator rit = m_mapModules.rbegin(); rit != m_mapModules.rend(); rit++)
	{
		if (nullptr != rit->second)
		{
			BaseModule* mModule = rit->second;

			mModule->GRelease();
		}
	}

	m_mapModules.clear();

	delete this;
}

BaseModule* GModuleManager::GetModule(const std::string & _key)
{
	XY_IF_FOUND_IN_MAP(_key, it, m_mapModules, MapModules)
	{
		BaseModule* _modules = it->second;

		return _modules;
	}

	return nullptr;
}

void GModuleManager::RegisterModule(const std::string & _key, BaseModule* _obj)
{
	XY_IF_FOUND_IN_MAP(_key, it, m_mapModules, MapModules)
	{
		return;
	}
	else
	{
		XY_MAP_INSERT(_key, _obj, m_mapModules);
	}
}

void GModuleManager::UnRegisterModule(const std::string & _key)
{
	XY_IF_FOUND_IN_MAP(_key, it, m_mapModules, MapModules)
	{
		XY_MAP_ERASE(_key, m_mapModules);
	}
}










