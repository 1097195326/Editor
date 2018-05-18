// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
/**
 * 
 */
class EDITOR_API AccessoriesComponent:public BaseComponent
{
public:
	AccessoriesComponent();

	virtual ~AccessoriesComponent();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnStartRender() override;

	virtual void OnStartSkeletalRender() override;

	void ChangePiecesCompont(int32 _targetTid) override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

private:
	void FindComponentId(int32 & _outStyleComponetRid, int32 & _outSuitComponetTid);

public:
	virtual void CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject) override;
};
