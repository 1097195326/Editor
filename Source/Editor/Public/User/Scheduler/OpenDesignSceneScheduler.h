// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MsgCenter.h"
#include "BaseScheduler.h"
/**
 * 
 */
class EDITOR_API OpenDesignSceneScheduler:public BaseScheduler
{
public:
	OpenDesignSceneScheduler();

	virtual ~OpenDesignSceneScheduler();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	virtual void Excute(TMap<FString, FString> & _params) override;

private:
	void ClickCallBack(msg_ptr _msg);
};
