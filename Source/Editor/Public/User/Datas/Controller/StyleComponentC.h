// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseC.h"

struct CAccessoryStruct
{
public:
	int32 m_styleAccessoryId = 0;

	int32 m_suitAccessoryId = 0;
};

struct SliceStruct
{
public:
	int32 m_styleSliceId = 0;

	int32 m_suitSliceId = 0;
};

struct StitchStruct
{
public:
	int32 m_styleStitchId = 0;

	int32 m_suitStitchId = 0;
};

/**
 * 
 */
class EDITOR_API StyleComponentC :public BaseC
{
public: //  R.
	TArray<CAccessoryStruct*> m_AccessoryIds;

	TArray<SliceStruct*> m_SliceIds;

	TArray<StitchStruct*> m_StitchsuitIds;

public:
	StyleComponentC();

	virtual ~StyleComponentC();

protected:
	// 子类可以重写.
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitData(int32 _cid, int32 _rid, int32 _tid) override;
public:
	virtual void AddJsonC(TSharedPtr<FJsonObject> & _jsonObj) override;

private:
	void CreateAccessory(int32 _rid,int32 _tid);

	void CreateSlice(int32 _rid, int32 _tid);

	void CreateStitch(int32 _rid, int32 _tid);
};
