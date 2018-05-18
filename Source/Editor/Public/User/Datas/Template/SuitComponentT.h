// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitComponentT:public BaseT
{
public:
	SuitComponentT();
	~SuitComponentT();
private:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	// ab款.
	int32 m_AdapterType;
	float m_CameraRotate = 0.0f;
	float m_cameraDistance = 0.0f;
	float m_cameraHeight = 0.0f;
	float m_cameraPitching = 0.0f;
	FString m_IconUrl;
	TArray<int32> m_SuitAccessoriesIds;
	TArray<int32> m_SuitSliceIds;
	TArray<int32> m_SuitStitchsuitIds;
};
