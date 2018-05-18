// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "LogicSceneState.h"

LogicSceneState::LogicSceneState()
{
}

void LogicSceneState::InitState(BaseLogicScene * _logicScene, int32 _state)
{
	m_ownerLogicScene = _logicScene;

	m_selfState = _state;
}

LogicSceneState::~LogicSceneState()
{

}
