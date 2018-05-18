// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EDITOR_API GPathTool
{
public:

	GPathTool();
	~GPathTool();

public:

	// 根据平台和相对路径获取指定资源完整路径.
	static FString GetSuitPath(const FString & _sourcePath);

	// 根据平台获取人台actor路径.
	static FString GetTableActorPath(const FString & _sourcePath);

	// 根据平台获取走秀actor路径.
	static FString GetWalkActorPath(const FString & _sourcePath);

	// 根据平台获取Fabric路径.
	static FString GetFabricPath(const FString & _sourcePath);

	// 根据平台获取BaseResource路径.
	static FString GetBaseResourcePath(const FString & _sourcePath);

	// 根据平台获取根目录.
	static FString GetPlatformPath(const FString & _sourcePath);

	// http截取域名后面的路径.
	static FString GetHttpPath(const FString & _httpUrl);
};
