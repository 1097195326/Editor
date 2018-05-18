// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BaseT.h"
#include "Serialization/JsonSerializer.h"
#include "UObject/NoExportTypes.h"

/**
 * 
 */

 // 通过pid直接获取Template对象.
typedef std::map<int32, BaseT*>			TeamplatePIDMap;

class GCORE_API GBaseTemplateManager:public ILoad
{
protected:

	// Template管理的字典.
	TeamplatePIDMap						m_templateMap;

private:

	// 保存模板数据的路径.
	FString    m_saveLocalPath = TEXT("");

public:

	GBaseTemplateManager();

	virtual ~GBaseTemplateManager();

	virtual void GLoad() final;

	virtual void GRelease() final;

	// 子类必须继承此方法.
	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

public:

	BaseT* GetTemplateByPID(int32 _pid);

	template<typename T>
	T* GetTemplateByPID(int32 _pid)
	{
		XY_IF_FOUND_IN_MAP(_pid, it, m_templateMap, TeamplatePIDMap)
		{
			BaseT* mPrototype = it->second;

			return (T*)mPrototype;
		}
		return nullptr;
	}

	// 释放掉所有资源.
	virtual void ReleaseAllData() final;
	 
	// 解析json内容.
	virtual void EncodeData(TSharedPtr<FJsonObject> & _content);

	// 添加数据到容器里.
	virtual void AddData(BaseT * _content) final;

};
