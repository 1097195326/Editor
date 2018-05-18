 // Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "AARefClasses.h"
#include "BaseModule.h"
#include "GModuleManager.h"

AARefClasses* AARefClasses::m_Instance = nullptr;
AARefClasses::AARefClasses()
{
}
AARefClasses::~AARefClasses()
{
}

void * AARefClasses::CreateClassByName(string _ClassName)
{
	map<string, PTRCreateObject>::const_iterator iter;
	iter = m_ClassMap.find(_ClassName);
	if (iter == m_ClassMap.end())
	{
		return nullptr;
	}
	else
	{
		return (iter->second)();
	}
}

void AARefClasses::LoadAllModule()
{
	// 遍历module并new.
	for (map<string, PTRCreateObject>::reverse_iterator rit = m_moduleMap.rbegin(); rit != m_moduleMap.rend(); rit++)
	{
		if (rit->second)
		{
			BaseModule * mModule = (BaseModule*)(rit->second)();

			string mClassName = rit->first;

			mModule->GLoad();

			GModuleManager::GetInstance()->RegisterModule(mClassName, mModule);
		}
	}
}

void AARefClasses::GRelease()
{
	/*m_ClassMap.clear();

	m_moduleMap.clear();

	m_Instance = nullptr;

	delete this;*/
}

void AARefClasses::RegistClass(string _Name, PTRCreateObject _Method)
{
	m_ClassMap.insert(pair<string, PTRCreateObject>(_Name, _Method));
}

void AARefClasses::RegistModuleClass(string _Name, PTRCreateObject _Method)
{
	m_moduleMap.insert(pair<string, PTRCreateObject>(_Name, _Method));

	m_ClassMap.insert(pair<string, PTRCreateObject>(_Name, _Method));
}

AARefClasses* AARefClasses::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new AARefClasses();
	}
	return m_Instance;
}


