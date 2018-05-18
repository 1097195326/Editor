// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"
/*
 *
 * 
 */
class EDITOR_API SuitAccessoriesT:public BaseT
{
public:
	SuitAccessoriesT();
	~SuitAccessoriesT();
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj) override;

public:
	FString m_CameraRotate;
	FString m_Code;
	FString m_IconUrl;
	FString m_ImageUrl;
	FString  m_State;
	int32 m_Status;
	TArray<int32> m_SuitComponent;
	FString useType;
};
