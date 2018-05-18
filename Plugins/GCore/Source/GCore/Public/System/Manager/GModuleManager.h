// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BaseModule.h"
#include "ILoad.h"


/**
 * 
 */
typedef	std::map<std::string, BaseModule*>                MapModules;

class GCORE_API GModuleManager:public ILoad
{
	
private:    // 变量区间.

			// 单利.
	static GModuleManager *          m_pInstance;

	MapModules                  m_mapModules;

public:
	GModuleManager();

	~GModuleManager();

	// 单利. 
	static GModuleManager * GetInstance();

	virtual void GLoad() override;
	virtual void GRelease() override;

	template<typename T>
	T* GetModule(const std::string & _key)
	{
		XY_IF_FOUND_IN_MAP(_key, it, m_mapModules, MapModules)
		{
			BaseModule* _modules = it->second;

			return (T*)_modules;
		}

		return nullptr;
	}

	BaseModule* GetModule(const std::string & _key);

	void RegisterModule(const std::string & _key, BaseModule* _obj);

	void UnRegisterModule(const std::string & _key);
	
	
};
