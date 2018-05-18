// Fill out your copyright notice in the Description page of Project Settings.

#include "GPathTool.h"
#include "GFileHelper.h"
#include "GController.h"

GPathTool::GPathTool()
{
}

GPathTool::~GPathTool()
{
}

FString GPathTool::GetSuitPath(const FString & _sourcePath)
{
	if (GController::m_platform == 0 || GController::m_platform == 1)
	{
		return FString::Printf(TEXT("/Contents/PC/Suits/%s"), *_sourcePath);
	}
	else
	{
		return FString::Printf(TEXT("/Contents/Mobile/Suits/%s"), *_sourcePath);
	}
}

FString GPathTool::GetTableActorPath(const FString & _sourcePath)
{
	if (GController::m_platform == 0 || GController::m_platform == 1)
	{
		return FString::Printf(TEXT("/Contents/PC/%s"), *_sourcePath);
	}
	else
	{
		return FString::Printf(TEXT("/Contents/Mobile/%s"), *_sourcePath);
	}
}

FString GPathTool::GetWalkActorPath(const FString & _sourcePath)
{
	if (GController::m_platform == 0 || GController::m_platform == 1)
	{
		return FString::Printf(TEXT("/Contents/PC/%s"), *_sourcePath);
	}
	else
	{
		return FString::Printf(TEXT("/Contents/Mobile/%s"), *_sourcePath);
	}
}

FString GPathTool::GetFabricPath(const FString & _sourcePath)
{
	if (GController::m_platform == 0 || GController::m_platform == 1)
	{
		return FString::Printf(TEXT("/Contents/PC/Fabric/%s"), *_sourcePath);
	}
	else
	{
		return FString::Printf(TEXT("/Contents/Mobile/Fabric/%s"), *_sourcePath);
	}
}

FString GPathTool::GetBaseResourcePath(const FString & _sourcePath)
{
	if (GController::m_platform == 0 || GController::m_platform == 1)
	{
		return FString::Printf(TEXT("/Contents/PC/BaseResource/%s"), *_sourcePath);
	}
	else
	{
		return FString::Printf(TEXT("/Contents/Mobile/BaseResource/%s"), *_sourcePath);
	}
}

FString GPathTool::GetPlatformPath(const FString & _sourcePath)
{
	if (GController::m_platform == 0 || GController::m_platform == 1)
	{
		return FString::Printf(TEXT("/Contents/PC/%s"), *_sourcePath);
	}
	else
	{
		return FString::Printf(TEXT("/Contents/Mobile/%s"), *_sourcePath);
	}
}

FString GPathTool::GetHttpPath(const FString & _httpUrl)
{
	FString presentDownloadPath = GFileHelper::ProjectPersistentDownloadDir();

	FString mLeft = _httpUrl;// http url.

	FString mRight; // 压缩后缀.

	if (!_httpUrl.Split(TEXT("?"), &mLeft, &mRight))
	{
		mLeft = _httpUrl;
	}

	// 截取第三个斜杠.
	FString mHttpHead;

	FString mHttpUrl;
	if (!mLeft.Split(TEXT("//"), &mHttpHead, &mHttpUrl))
	{
		mHttpUrl = mLeft;
	}
	FString mLocalPath; // 压缩后缀.

	if (!mHttpUrl.Split(TEXT("/"), &mHttpHead, &mLocalPath))
	{
		mLocalPath = mHttpUrl;
	}

	FString mSavePath = FString::Printf(TEXT("%s/%s"), *presentDownloadPath, *mLocalPath);
	
	return mSavePath;
}
