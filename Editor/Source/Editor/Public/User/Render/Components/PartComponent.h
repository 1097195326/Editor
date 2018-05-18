// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"

struct InheritFabricStruct
{
public:
	// 面料ID.
	int32 m_suitFabricId = 0;

	// 花型Id.
	FString m_suitPatternId;

	// 颜色id.
	int32 m_suitColorId = 0;

	// 旋转角度.
	float m_angle = 0.0f;

	// 偏移.
	FVector2D m_offset;

	// uv缩放.
	FVector2D m_uVScaleUser;
};

/**
 * 
 */
class EDITOR_API PartComponent :public BaseComponent
{
public:
	PartComponent();

	virtual ~PartComponent();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnStartRender() override;

	virtual void OnStartSkeletalRender() override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

public:

	// 换部件的方法.
	virtual void ChangePiecesCompont(int32 _targetTid) override;

	// 通过AB款被动切换部件.
	void ChangePiecesComponentByAdapterType(int32 _AdapterType);

	virtual void CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject) override;

private:

	// 继承面料的方法.
	void InheritFabric(IComponent * mComponent,InheritFabricStruct & mInheritFabricStruct);
};
