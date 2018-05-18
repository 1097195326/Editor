// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"



/**
 * 渲染相关的.
 */
class GCORE_API IComponent:public ILoad
{


public:

	IComponent();

	virtual ~IComponent();


	virtual void GLoad() =0;

	virtual void GRelease() =0;

	virtual void InitData(int32 _rid, int32 _tid,int32 _cid, IComponent * _parentComponent) =0;

	virtual IComponent * GetParentComponent() =0;

	virtual TArray<IComponent*> & GetChildComponents() =0;

	virtual void StartRenderChildComponent() =0;

	virtual void StartSkeletalRenderChildComponent() =0;

	virtual IComponent * GetComponentByName(const FString & _componentName) = 0;

	virtual void GetComponentByTag(int32 _tag,TArray<IComponent*> & _childComponentArray) = 0;

	virtual IComponent * GetComponentByCID(int32 _tag) = 0;

	virtual IComponent * GetParentComponentByTag(int32 _tag) =0;

	virtual int32 GetComponentTag() = 0;

	virtual void RemoveChildComponent(IComponent * _component) =0;

	// 开始渲染设计Actor.
	virtual void StartDesignRender() =0;

	// 开始渲染走秀actor.
	virtual void StartWalkingRender() =0;

	virtual void ReleaseRender() =0;

};
