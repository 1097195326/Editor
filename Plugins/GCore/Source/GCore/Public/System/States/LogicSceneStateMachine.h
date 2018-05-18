// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IStateMachine.h"
#include "LogicSceneState.h"
#include "BaseLogicScene.h"
/**
 * 场景用的状态机.
 */
class GCORE_API LogicSceneStateMachine:public IStateMachine
{
protected:
	int32 m_currentState = 0;

	BaseLogicScene * m_ownerLogicScene = nullptr;

	TMap<int32,LogicSceneState*> m_stateMap;

public:
	LogicSceneStateMachine();

	virtual ~LogicSceneStateMachine();

public:
	virtual int32 GetCurrentState() final;

	virtual void InitStateMachine(BaseLogicScene * _ownerLogicScene) final;

	virtual void GRelease() final;

public:
	virtual void OnInitStateMachine()=0;

	virtual void OnRelease() = 0;
};
