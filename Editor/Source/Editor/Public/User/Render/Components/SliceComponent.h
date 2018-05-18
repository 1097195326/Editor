// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MsgCenter.h"
#include "Runtime/Core/Public/Templates/UnrealTemplate.h"
#include "BaseComponent.h"
/**
 * 
 */
class EDITOR_API SliceComponent :public BaseComponent
{
private:
	FString m_showFolder = TEXT("Show");

	FStringAssetReference m_staticMeshAsset;

	// 当前片是否正在验证继承花型.
	bool m_isCheckingPatternExtern = false;

public:
	UTexture * m_baseTexture = nullptr;

	UTexture * m_normalTexture = nullptr;

public:
	SliceComponent();

	virtual ~SliceComponent();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnReleaseRender() override;

	virtual void OnStartSkeletalRender() override;

	virtual void OnStartRender() override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

public:
	// 创建辅料.
	void CreateAccessoriesComponents();

	// 创建面料.
	void CreateFabricComponent();

	// 创建花型.
	void CreatePatternComponent();

	// 切换花型(片需要重写).
	virtual void ChangePatternComponent(FString _targetTid) override;

	// 切换花色(片需要重写).
	virtual void ChangeColorComponent(int32 _targetTid) override;

	// 切换面料(片需要重写).
	virtual void ChangeFabricComponent(int32 _targetTid) override;

	// 获取所有的子组件里用到的花型(部片类需要重写).
	virtual void GetAllPatternInChildComponent(TArray<UsedFabricStruct> & _array) override;

	virtual void GetAllFabricInfoIn(TArray<UsedFabricStruct> & _array) override;

	// 根据struct修改花型面料花色(部片需要重写).
	virtual void ChangeRenderTargetTid(UsedFabricStruct & _usedFabricStruct) override;

	void UpdateChildComponent();

	// 重置高亮.
	void ResetOutLine();

	// 显示高亮.
	void SetOutLine();

	// 其他的颜色都设置透明.
	void SetAlpa();

	virtual void CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject) override;

	virtual FString GetPatternId() override;

	// 拍照时候不需要受光(仅限于windows).
	void SetTrueLightChannel();

	// 拍完照需要受光(仅限于windows).
	void SetFalseLightChannel();
private:
	USkeletalMesh * GetShowSkeletalMesh(const FString & inUrl) const;

	// 过滤交叉面料.
	void GetSameFabricTid(UsedFabricStruct & _usedFabricStruct);

	// 检查面料是否给线迹或者辅料使用.
	bool CheckFabricIsStitchOrAccessories();

	// 判断哪些面料开启受光影响.
	bool CheckLightFabric();

};
   