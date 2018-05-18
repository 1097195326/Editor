// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "PrototypeManager.h"
#include "GController.h"
#include "GameDirector.h"
#include "GSceneceManager.h"

GSceneceManager::GSceneceManager()
{
}

GSceneceManager::~GSceneceManager()
{
}

void GSceneceManager::GLoad()
{
	if (GController::m_platform == ENUM_PLATFORM::GWINDOWS)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(100);
	}
	else if (GController::m_platform == ENUM_PLATFORM::GMAC)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(101);
	}
	else if (GController::m_platform == ENUM_PLATFORM::GANDROID)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(102);
	}
	else if (GController::m_platform == ENUM_PLATFORM::GIOS)
	{
		m_scenePrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ScenePrototype>(103);
	}
	
}

void GSceneceManager::GRelease()
{

}

void GSceneceManager::OpenScene(const FString & _sceneName)
{
	if (nullptr != m_scenePrototype)
	{
		FString mScenePath = m_scenePrototype->GetLevelName(_sceneName);

		UGameplayStatics::OpenLevel(UGameDirector::GetInstance(), *mScenePath);
	}
}
