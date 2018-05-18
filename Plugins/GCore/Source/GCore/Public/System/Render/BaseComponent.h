// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IComponent.h"
#include "BaseC.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "BaseR.h"
#include "GBaseRender.h"
#include "Serialization/JsonSerializer.h"
#include "BaseT.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseActor.h"

enum EnumComponentType
{
	STYLE_COMPONENT=0, // 款.
	PART_COMPONENT, // 部位.
	PIECES_COMPONENT, // 部件.
	SLICE_COMPONENT, // 部片.
	STITCH_COMPONENT, // 线迹.
	PATTERN_COMPONENT, // 花型.
	FABRIC_COMPONENT, // 面料.
	FABRICCOLOR_COMPONENT, // 花色.
	ACCESSORIES_COMPONENT, // 辅料.
};

// 查找所用的花型或者花色的结构体.
struct UsedFabricStruct
{
public: // 原数据.
	int32 m_fabricTid = 0;

	int32 m_colorTid = 0;

	FString m_patternTid;

	TArray<int32> m_fabricTidArray;

public: //  要替换的数据.
	int32 m_targetFabricTid = 0;

	int32 m_targetColorTid = 0;

	FString m_targetPatternTid;
	//重载==运算符.
	bool operator ==(const UsedFabricStruct &b)
	{
		if (b.m_colorTid==m_colorTid && b.m_fabricTid==m_fabricTid && b.m_patternTid==m_patternTid)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
/**
 * 
 */
class GCORE_API BaseComponent:public IComponent
{

protected:
	int32 m_cid = 0;

	// component 类型.
	int32 m_componentTag = (int32)EnumComponentType::STYLE_COMPONENT;

	FString m_componentName;

	GBaseRender * m_owner;

	BaseC * m_dataC;

	// 获取父类.
	IComponent * m_parentComponent = nullptr;

	// 获取所有子类.
	TArray<IComponent*> m_childComponents;

public:
	// 静态模型.
	UStaticMeshComponent * m_staticMeshComponent = nullptr;

	USkeletalMeshComponent * m_skeletalMeshComponent = nullptr;

public:
	BaseComponent();

	virtual ~BaseComponent();

public: // set get方法.

	virtual void SetRender(GBaseRender * _render) final;

	virtual void SetComponentName(const FString & _componentName) final;

	virtual FString GetComponentName() final;

	virtual void SetComponentTag(int32 _tag) final;

	virtual int32 GetComponentTag() final;

	virtual int32 GetComponentCID() final;

	virtual GBaseRender * GetOwner() final;


protected:

	virtual void StartRenderChildComponent() final;

	virtual void StartSkeletalRenderChildComponent() final;

public:

	virtual void GLoad() final;

	virtual void GLoad(GBaseRender * _render) final;

	virtual void GRelease() final;

	virtual BaseC * GetDataC() final;

	virtual void GetAllTexturePath(TArray<FString> & _pathArray) final;

	virtual void GetAllColorTexture(TArray<FString> & _pathArray) final;

	virtual void GetAllPatternIds(TArray<FString> & _patternIds) final;

	virtual void InitData(int32 _rid, int32 _tid,int32 _cid, IComponent * _parentComponent) final;

	virtual IComponent * GetParentComponent() final;

	virtual IComponent * GetParentComponentByTag(int32 _tag) final;

	virtual TArray<IComponent*> & GetChildComponents() final;

	virtual void AddChildComponent(IComponent * _component) final;

	virtual IComponent * GetComponentByName(const FString & _componentName) final;

	virtual void GetComponentByTag(int32 _tag, TArray<IComponent*> & _childComponentArray) final;

	virtual IComponent * GetComponentByCID(int32 _cid) final;

	virtual void RemoveChildComponent(IComponent * _component) final;

	// 开始渲染设计Actor.
	virtual void StartDesignRender() final;

	// 开始渲染走秀actor.
	virtual void StartWalkingRender() final;

	// 释放掉渲染相关的资源.
	virtual void ReleaseRender() final;

	// 通过Tid获取具体的component.
	virtual BaseComponent * GetComponentByTid(int32 _tid, TArray<EnumComponentType> & _componentType) final;

	virtual void ReleaseComponents() final;

private:
	virtual bool CheckTag(TArray<EnumComponentType> & _componentType) final;

protected:

	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

	virtual void OnReleaseRender();

	virtual void OnStartRender() = 0;

	virtual void OnStartSkeletalRender() = 0;

	// 要是花型的话把花型路径返回.
	virtual FString GetPatternPath();

	// 获取花色的贴图路径.
	virtual TArray<FString> GetColorTexture();

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) = 0;

public:
	// 切换部件(部位组件需要重写).
	virtual void ChangePiecesCompont(int32 _targetTid);

	// 切换花型(片需要重写).
	virtual void ChangePatternComponent(FString _targetTid);

	// 切换花色(片需要重写).
	virtual void ChangeColorComponent(int32 _targetTid);

	// 切换面料(片需要重写).
	virtual void ChangeFabricComponent(int32 _targetTid);

	// 获取所有的子组件里用到的花型(部片类需要重写).
	virtual void GetAllPatternInChildComponent(TArray<UsedFabricStruct> & _array);

	virtual void GetAllFabricInfoIn(TArray<UsedFabricStruct> & _array);

	// 批量修改片上用的花型面料花色(部件组件需要重写).
	virtual void ChangeSliceChildComponent(UsedFabricStruct & _usedFabricStruct);

	// 根据struct修改花型面料花色(部片需要重写).
	virtual void ChangeRenderTargetTid(UsedFabricStruct & _usedFabricStruct);

	// 包装数据.
	virtual void CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject);

	// 获取花型Id.
	virtual FString GetPatternId();

};
