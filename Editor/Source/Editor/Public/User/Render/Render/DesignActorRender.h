// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Walker.h"
#include "Catwalks.h"
#include "GBaseRender.h"

class EDITOR_API DesignActorRender :public GBaseRender
{
private:
	AWalker * m_walker = nullptr;

	// 是否第一次渲染，判断摄像机初始化用.
	bool m_isFirstRender = true;

public:
	DesignActorRender();

	virtual ~DesignActorRender();

protected:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	// 开始渲染.
	void StartRender();

	void StartSkeletalRender();

	// 获取用到的花型路径.
	void GetAllPattern(TArray<FString> & _pattenPathArr);

	// 获取用到的花型id.
	void GetAllPatternIds(TArray<FString> & _pattenPathArr);

	// 下载花型.
	void DownloadPattern();

	// 移除模型渲染相关的东西.
	void RemoveRender();

	// 获取走秀的模型.
	AWalker * GetPawn();

	// 根据面料ID找出他的部位是线迹还是辅料还是成衣.
	EnumComponentType GetComponentTypeByFabricId(int32 _fabricId);

private:
	// 下载花型以后开始渲染.
	void StartRenderOnCheckPattern();

	void DownloadPatternProcess(FString, int32, int32);

	void DownloadPatternFinish();

	// 获取走秀actor路径.
	FString GetCatWalkActorPath();

	// 获取人台actor路径.
	FString GetTableActorPath();
	
};