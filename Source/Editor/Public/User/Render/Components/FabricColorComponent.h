// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
/**
 * 
 */
class EDITOR_API FabricColorComponent :public BaseComponent
{
public:
	FLinearColor m_color = FLinearColor::White;

	FString m_hexRgb;

	TArray<UTexture2D*> m_textureArray;

public:

	FabricColorComponent();

	virtual ~FabricColorComponent();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnReleaseRender() override;

	virtual void OnStartRender() override;

	virtual void OnStartSkeletalRender() override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

public:
	virtual TArray<FString> GetColorTexture() override;

public:

	FString GetMaterialUri();


private:
	void LoadTexture();
};
