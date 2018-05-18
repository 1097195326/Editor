// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BaseR.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <functional>
#include "Serialization/JsonSerializer.h"
#include "UObject/NoExportTypes.h"

/**
 * 
 */
 // 通过pid直接获取runtime对象.
typedef std::map<int32, BaseR*>			RuntimePIDMap;

typedef std::function<void(FString _errorInfo)> LoadErrorDelegate;

typedef std::function<void(int32 _pageIndex,int32 _pageSize,int32 _recordCount,int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData)> LoadFinishDelegate;

// 主要是给主界面UI发送消息用.
class GCORE_API LoadUserStyleData
{
public:
	LoadUserStyleData();

	bool _msgSuccess = false;

	int32 _pageIndex = 0;
	
	int32 _pageSize = 0;
	
	int32 _recordCount = 0;
	
	int32 _totalPage = 0;
	
	TArray<BaseR*> _ArrayRuntimData;
};


class GCORE_API GBaseRuntimeManager :public ILoad
{
protected:

	// runtime管理的字典.
	RuntimePIDMap		 m_runtimeMap;

	// 加载数据异常回调.
	LoadErrorDelegate    m_loadErrorDelegate =nullptr;

	// 加载数据成功的回调.
	LoadFinishDelegate   m_loadFinishDelegate = nullptr;

public:
	
	GBaseRuntimeManager();

	virtual ~GBaseRuntimeManager();

	virtual void GLoad() final;

	virtual void GRelease() final;

	// 子类必须继承此方法.
	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

	// 绑定回调.
	template<typename T>
	void BindLoadErrorDelegate(T* obj, void (T::*_func)(FString _errorInfo))
	{
		if (obj!=nullptr)
		{
			m_loadErrorDelegate = LoadErrorDelegate(std::bind(_func, obj, std::placeholders::_1));
		}
		
	}

	// 绑定回调.
	template<typename T>
	void BindLoadFinishDelegate(T* obj, void (T::*_func)(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData))
	{
		m_loadFinishDelegate = LoadFinishDelegate(std::bind(_func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	}

	virtual void ClearBind() final;

public:

	BaseR* GetRuntimeByPID(int32 _pid);

	template<typename T>
	T* GetRuntimeByPID(int32 _pid)
	{
		XY_IF_FOUND_IN_MAP(_pid, it, m_runtimeMap, RuntimePIDMap)
		{
			BaseR* mPrototype = it->second;

			return (T*)mPrototype;
		}
		return nullptr;
	}

	virtual void ReleaseAllData() final;

	// 解析json内容.
	virtual void EncodeData(TSharedPtr<FJsonObject> & _content);

	// 添加数据到容器里.
	virtual void AddData(BaseR * _rData) final;
};
