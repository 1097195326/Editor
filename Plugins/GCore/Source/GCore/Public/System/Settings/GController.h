// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


// 自定义平台设置.
enum ENUM_PLATFORM
{
	GWINDOWS = 0,
	GMAC,
	GANDROID,
	GIOS
};
enum ENUM_LANGUAGE
{
	GCHINESE = 0,
	GENGLISH,
};
/**
 *	2018/1/13 -gx.
 *	
 *	系统设置相关的类.
 *	是否debug模式.
 *	屏幕分辨率设置等.
 *  RunPlatform 运行的平台.
 *	oss 相关信息等.
 */
class GCORE_API GController
{
public:
	GController();
	~GController();

public:
	// 屏幕分辨率1.78 1.66等等.
	static FString m_proportion;

	// 是否debug.
	static bool   m_isDebug;

	// 正在运行的平台.
	static ENUM_PLATFORM m_platform;

	// 语言.
	static ENUM_LANGUAGE m_language;

	// app版本号.
	static FString m_appVersion;

	// 屏幕宽.
	static int32 m_screenWidth;

	// 屏幕高.
	static int32 m_screenHeight;

	// 是否正式适配UI.
	static bool m_isadaptation;

	// 是否正式包.
	static bool m_isAdmin;
};
