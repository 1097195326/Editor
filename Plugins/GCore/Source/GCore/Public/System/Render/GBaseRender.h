// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRender.h"
#include "UITextureManager.h"
#include "BaseActor.h"
#include "IComponent.h"
#include "ILoad.h"
/**
 * 渲染类.
 */
class GCORE_API GBaseRender:public IRender, public ILoad
{
protected:
	ABaseActor * m_actor = nullptr;

	IComponent * m_rootComponent = nullptr;

	UUITextureManager * m_textureManager = nullptr;

private:
	int32 m_cid = 0;

public:
	GBaseRender();  

	virtual ~GBaseRender();

public:

	virtual void GLoad() final;

	virtual void GRelease() final;

	virtual ABaseActor * GetActor() final;

	virtual IComponent * GetRootComponent() final;

	virtual int32 CreateComponentID() final;

	virtual UUITextureManager * GetTextureManager() final;

protected:

	virtual void OnLoad() = 0;

	virtual void OnRelease() = 0;

};
