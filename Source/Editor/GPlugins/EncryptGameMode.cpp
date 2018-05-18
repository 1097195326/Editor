// Fill out your copyright notice in the Description page of Project Settings.

#include "EncryptGameMode.h"
#include "GFileHelper.h"
#include "Editor.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "EncryptDecryptTool.h"
#include "GPlatformFileManager.h"



void AEncryptGameMode::OnBeginPlay()
{
	// 加密.
	TArray<FString> mXmlArray;

	FString mContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());

	FString mLocalResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resources/Config/"));

	GPlatformFileManager::ScanDirectory(mXmlArray, mLocalResourcePath, TEXT("*.xml"));

	FString mSaveResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resource/Config/"));

	// 加密.
	for (int32 i = 0; i < mXmlArray.Num(); i++)
	{
		FString mXmlPath = mXmlArray[i];

		FString mContent;

		GFileHelper::LoadFileToString(mContent, *mXmlPath);

		FString mEncrypt = mContent;
		// 加密.
		if (!mXmlPath.Contains(TEXT("ChinesePrototype.xml")) && !mXmlPath.Contains(TEXT("EnglishPrototype.xml")))
		{
			mEncrypt = FBase64::Encode(mContent);

			mEncrypt = EncryptAndDecryptTool::Encrypt(mEncrypt, EncryptKey);
		}

		FString mSavePath = FString::Printf(TEXT("%s%s.%s"), *mSaveResourcePath, *(FPaths::GetBaseFilename(mXmlPath)), *(FPaths::GetExtension(mXmlPath)));

		GFileHelper::SaveStringToFile(mEncrypt, *mSavePath);
	}
}

void AEncryptGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
}

FString AEncryptGameMode::GetEncodeStr(FString _content)
{
	//FString mContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());

	//FString mLocalResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resource/Config/ChinesPrototype.xml"));

	//GFileHelper::LoadFileToString(_content, *mLocalResourcePath);

	//FString mEncrypt = FBase64::Encode(_content);
	////FString mEncrypt = EncryptAndDecryptTool::Encrypt(_content, EncryptKey);

	//mLocalResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resources/Config/encode.xml"));
	//GFileHelper::SaveStringToFile(mEncrypt, *mLocalResourcePath);

	//UE_LOG(GEditorLog, Error, TEXT("http response content %s"), *mEncrypt);
	//return mEncrypt;
	return TEXT("");
}

