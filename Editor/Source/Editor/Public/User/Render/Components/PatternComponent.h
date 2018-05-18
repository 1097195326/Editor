// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
/**
 * 
 */
class EDITOR_API PatternComponent :public BaseComponent
{
public:
	UTexture2D * m_texture2d = nullptr;

	float m_width = 0.0f;

	float m_height = 0.0f;

	FString m_patternPath;

	FString m_patternTid = TEXT("");

public:
	PatternComponent();

	virtual ~PatternComponent();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnReleaseRender() override;

	virtual void OnStartRender() override;

	virtual void OnStartSkeletalRender() override;

	virtual FString GetPatternPath() override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

public:
	void ChangePatternTid(const FString & _tid);

	float GetWidthT();

	float GetHeightT();

	FVector2D GetUVScaleUser();

	FString GetPatternId();
};
