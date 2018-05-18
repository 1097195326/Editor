// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLogicScene.h"
/**
 * 
 */
class EDITOR_API LogicLoginScene:public BaseLogicScene
{
public:
	LogicLoginScene();
	virtual ~LogicLoginScene();  

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	// 跳转主场景.
	void JoinMainScene();

private:

	void OnDownloadUserPatternSuccess(msg_ptr _msg);
	void OnLoadHomeFinish(msg_ptr _msg);
};
