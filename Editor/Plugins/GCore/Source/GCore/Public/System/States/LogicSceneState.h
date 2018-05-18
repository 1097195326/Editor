// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IState.h"
#include "BaseLogicScene.h"
/**
 * 场景用的状态.
 */
class GCORE_API LogicSceneState:public IState
{
protected:
	int32 m_selfState = 0;

	BaseLogicScene * m_ownerLogicScene = nullptr;

public:
	LogicSceneState();

	virtual~LogicSceneState();

public:

	virtual void InitState(BaseLogicScene * _logicScene,int32 _state) final;

};
