// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePrototype.h"
#include "PrototypeConst.h"
#include "castle_std.h"

// 通过pid直接获取prototype对象.
typedef std::map<int32, BasePrototype*>			PrototypePIDMap;


/**
 * 
 */
class GCORE_API PrototypeManager:public ILoad
{
public:
	PrototypeManager();

	virtual ~PrototypeManager();

	// 单利. 
	static PrototypeManager * GetInstance();

public:

	static PrototypeManager *           m_pInstance;

	// prototype管理的字典.
	PrototypePIDMap						m_protypeMap;

public:

	virtual void GLoad() final;

	virtual void GRelease() final;

	BasePrototype* GetPrototypeByPID(int32 _pid);

	// 自动加载prototype.
	bool LoadPrototype(const FString & _xmlPath);

	template<typename T>
	T* GetPrototypeByPID(int32 _pid)
	{
		XY_IF_FOUND_IN_MAP(_pid, it, m_protypeMap, PrototypePIDMap)
		{
			BasePrototype* mPrototype = it->second;

			return (T*)mPrototype;
		}
		return nullptr;
	}

	// 加载所有系统设置.
	void LoadAllSystemPrototype();
};
