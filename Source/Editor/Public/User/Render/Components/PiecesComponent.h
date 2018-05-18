// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
/**
 * 
 */
class EDITOR_API PiecesComponent :public BaseComponent
{
public:
	
	PiecesComponent();

	virtual ~PiecesComponent();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnStartRender() override;

	virtual void OnStartSkeletalRender() override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

private:
	// 创建线迹组件.
	void CreateStitchComponents();

	// 创建部片组件.
	void CreateSliceComponents();

	// 创建辅料组件.
	void CreateAccessoriesComponents();

public:

	// 批量修改片上用的花型面料花色(部件组件需要重写).
	virtual void ChangeSliceChildComponent(UsedFabricStruct & _usedFabricStruct) override;

	virtual void CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject) override;

	void CreateStitchJson(const TSharedPtr<FJsonObject> & _saveInfoObject);

	void CreateAccessoriesJson(const TSharedPtr<FJsonObject> & _saveInfoObject);
};
