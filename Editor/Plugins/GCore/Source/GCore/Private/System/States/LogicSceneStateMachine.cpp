// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "LogicSceneStateMachine.h"

LogicSceneStateMachine::LogicSceneStateMachine()
{
}

LogicSceneStateMachine::~LogicSceneStateMachine()
{
}

int32 LogicSceneStateMachine::GetCurrentState()
{
	return m_currentState;
}

void LogicSceneStateMachine::InitStateMachine(BaseLogicScene * _ownerLogicScene)
{
	m_ownerLogicScene = _ownerLogicScene;

	OnInitStateMachine();
}

void LogicSceneStateMachine::GRelease()
{
	OnRelease();

	for (auto& Elem : m_stateMap)
	{
		LogicSceneState * mState = Elem.Value;

		mState->GRelease();
	}

	m_stateMap.Empty();

	delete this;
}
