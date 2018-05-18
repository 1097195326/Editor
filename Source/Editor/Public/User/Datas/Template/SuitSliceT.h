// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/**
 * 
 */
class EDITOR_API SuitSliceT:public BaseT
{
public:
	SuitSliceT();
	~SuitSliceT();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	float m_CameraRotate = 0.0f;
	float m_cameraDistance = 0.0f;
	float m_cameraHeight = 0.0f;
	float m_cameraPitching = 0.0f;
	int32 m_FabricLibId;
	FString m_MeshUrl;
	float m_OriginalSize;
	FString m_PatternDefaultId;
	FString m_baseTextureUrl = TEXT("");
	FString m_meshNormalUrl;
	FString m_IconUrl;
	bool m_isRib = false;
};
