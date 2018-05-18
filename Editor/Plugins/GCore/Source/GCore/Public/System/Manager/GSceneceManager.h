// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "ScenePrototype.h"
/**
 * 2018/1/15 -gx
 * 只负责根据配置文件打开不同的场景.
 */
class GCORE_API GSceneceManager: public ILoad
{
private:
	ScenePrototype * m_scenePrototype = nullptr;

public:
	GSceneceManager();
	virtual ~GSceneceManager();

	virtual void GLoad() override;

	virtual void GRelease() override;

	void OpenScene(const FString & _sceneName);
};
