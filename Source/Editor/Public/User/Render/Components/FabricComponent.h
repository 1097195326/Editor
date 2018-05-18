// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "PatternComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

enum GEnumUseColorResource
{
	USE_COLOR_RGB=0, // 用花色的颜色.
	USE_COLOR_MATERIAL, // 用花色的材质.
	USE_COLOR_TEXTURE, // 用花色文理.
};
/**
 * 
 */
class EDITOR_API FabricComponent :public BaseComponent
{
private:
	int32 m_colorTextureCount = 0;
public:
	UMaterialInterface * m_material = nullptr;

	TArray<UMaterialInstanceDynamic*> m_materiaInstanceArray;

	UTexture * m_normalTexture = nullptr;

	UTexture * m_maskTexture = nullptr;

	UTexture2D * m_metal = nullptr;

	UTexture2D * m_roughness = nullptr;

	UTexture2D * m_basePattern = nullptr;

	// 判断使用花色还是花型.
	bool m_usedFabricColor = false;

	FString m_currentMaterialPath = TEXT("");

public:
	FabricComponent();

	virtual ~FabricComponent();

public:
	// 初始化材质.
	void InitMaterial(UMaterialInstanceDynamic * mMaterial);

	// 替换花型.
	void UpdateMateriasTexture(PatternComponent * _patternComponent, UMaterialInstanceDynamic * _material, float _originalSize);

	void UpdateUvScale(FVector2D _uvScale, UMaterialInstanceDynamic * _material);

	// 判断是否线迹辅料.
	void UpdateStitchFabric(UTexture * _baseTexture, UTexture * _normalTexture, UMaterialInstanceDynamic * _material);

	UTexture2D * GetBlankTexture();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void OnReleaseRender() override;

	virtual void OnStartRender() override;

	virtual void OnStartSkeletalRender() override;

	// 负责创建子组件等功能.
	virtual void OnInitData(int32 _rid, int32 _tid, int32 _cid) override;

private:
	FString GetFabricPath();

	void GetColorTexturePath(TArray<FString> & _pathArray);

	GEnumUseColorResource CheckUseColorMat();

	BaseComponent * GetFabricColorComponent();

	void SetDefaultColorTexture(UMaterialInstanceDynamic * _material);
};
