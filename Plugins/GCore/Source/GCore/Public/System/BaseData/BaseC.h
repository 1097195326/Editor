// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseR.h"
#include "BaseT.h"
#include "GObject.h"

/**
 * 
 */
class GCORE_API BaseC : public GObject, public ILoad
{
	
protected:
	int32 m_cid = 0;

	int32 m_rid = 0;

	int32 m_tid = 0;

	BaseT * m_dataT = nullptr;

public:
	// 渲染资源.
	FString m_renderAsset;
	
public:
	BaseC();
	virtual ~BaseC();
	
public:
	// set get方法.
	int32 GetCid();
	
	void  SetCid(int32 _cid);

	void SetTid(int32 _tid);

	int32 GetTid();

	BaseT * GetBaseT();

	template<typename T>
	T* GetBaseT()
	{
		if (nullptr != m_dataT)
			return (T*)m_dataT;
		return nullptr;
	}


public:
	// 子类不允许重写.
	virtual void GLoad() final;

	virtual void GRelease() final;

	virtual void InitData(int32 _cid,int32 _rid,int32 _tid) final;

	virtual void ChangeTid(int32 _tid) final;

protected:
	// 子类可以重写.
	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) = 0;

public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) = 0;
};
