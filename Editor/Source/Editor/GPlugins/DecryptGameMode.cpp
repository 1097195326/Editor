// Fill out your copyright notice in the Description page of Project Settings.

#include "DecryptGameMode.h"
#include "GFileHelper.h"
#include "EncryptDecryptTool.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "GPlatformFileManager.h"



void ADecryptGameMode::OnBeginPlay()
{
	// 加密.
	TArray<FString> mXmlArray;

	FString mContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());

	FString mLocalResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resource/Config/"));

	GPlatformFileManager::ScanDirectory(mXmlArray, mLocalResourcePath, TEXT("*.xml"));

	FString mSaveResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("DecodeResource/Config/"));

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
			FString mEncryptStr = EncryptAndDecryptTool::Decrypt(mContent, EncryptKey);
			FBase64::Decode(mEncryptStr, mEncrypt);
			
		}

		FString mSavePath = FString::Printf(TEXT("%s%s.%s"), *mSaveResourcePath, *(FPaths::GetBaseFilename(mXmlPath)), *(FPaths::GetExtension(mXmlPath)));

		GFileHelper::SaveStringToFile(mEncrypt, *mSavePath);
	}
}

void ADecryptGameMode::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

FString ADecryptGameMode::GetDecyptStr(FString _content)
{
	FString mContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());

	FString mLocalResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resources/Config/encode.xml"));

	//FString mEncrypt = EncryptAndDecryptTool::Decrypt(_content, EncryptKey);
	FString mContent;
	GFileHelper::LoadFileToString(mContent, *mLocalResourcePath);

	FString mEContent = mContent;
	FBase64::Decode(mContent, mEContent);

	const ANSICHAR* Source = TCHAR_TO_ANSI(*mEContent);


	mLocalResourcePath = FString::Printf(TEXT("%s%s"), *mContentDir, TEXT("Resource/Config/encode.xml"));
	
	GFileHelper::SaveStringToFile(mEContent, *mLocalResourcePath);

	UE_LOG(GEditorLog, Error, TEXT("http response content %s"), *mEContent);
	return mEContent;

}

