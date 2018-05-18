// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SliceComponent.h"
#include "BaseComponent.h"
/**
 * 
 */
class EDITOR_API StyleComponent :public BaseComponent
{
public:
	StyleComponent();

	virtual ~StyleComponent();

private:
	TArray<UStaticMeshComponent *> m_defaultModelComponentArray;

	TArray<USkeletalMeshComponent *> m_defaultModelSkeletalComponentArray;

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnReleaseRender() override;

	virtual void OnStartRender() override;

	virtual void OnStartSkeletalRender() override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

	virtual void CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject) override;

public:
	void ChangeAdapterType(int32 _adapterType);

private:
	// 静态模型赋值材质.
	void SetStaticMeshComponentMaterial();

	// 动态模型赋值材质.
	void SetSkeletalMeshComponentMaterial();

	// 获取主部位的主片.
	SliceComponent * GetMainSliceComponent();

	USkeletalMesh * GetShowSkeletalMesh(const FString & inUrl) const;
};
