// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseT.h"

// 拍照摄像机设置参数.
struct CameraPhotoCover
{
	float m_cameraDistance = 0.0f;

	float m_cameraHeight = 0.0f;

	float m_rotateAngle = 0.0f;

	// 拍照时候是否显示人台.
	int32 m_isDisplayModel = 1;

	float m_cameraPitching = 0.0f;
};

// 设计场景摄像机设置.
struct CameraDesignCover
{
	float m_cameraDistance = 0.0f;

	float m_cameraHeight = 0.0f;

	float m_rotateAngle = 0.0f;

	// 拍照时候是否显示人台.
	int32 m_isDisplayModel = 1;

	float m_cameraPitching = 0.0f;
};

/**
 * 
 */
class EDITOR_API SuitT:public BaseT
{
public:

	CameraPhotoCover m_cameraPhotoCover;

	CameraDesignCover m_cameraDesignCover;

	// 分类ID.
	int32 m_catId = 0;

	// icon
	FString m_iconUrl;

	// 父分类id.
	int32  m_parentCatId = 0;

	// 走秀模特模型.
	FString m_showModel;

	// 走秀动画.
	FString m_showModelAnimation;

	// 排序.
	int32 m_sort = 0;

	// 
	int32 m_status = 0;

	// 部位id集合.
	TArray<int32> m_ArrayPartId;

	// 默认显示的模型.
	TArray<FString> m_defaultModelArray;

	// 人台.
	FString m_tableModel;

	// 设计场景.
	FString m_design;

	// 走秀场景.
	FString m_catwalk;

	// sequenc动画.
	FString m_walkSequence;

public:
	
	SuitT();

	virtual~SuitT();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

protected:
	virtual void OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj);

};
