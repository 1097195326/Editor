// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FileHelper.h"

/**
 * 文件管理类,为了兼容ios io操作.
 */

class GCORE_API GFileHelper
{
public:
	/**
	* Load a binary file to a dynamic array.
	*/
	static bool LoadFileToArray(TArray<uint8>& _outArray, const TCHAR* _texPath, uint32 Flags = 0);

	/**
	* Load a text file to an FString.
	* Supports all combination of ANSI/Unicode files and platforms.
	* @param Result string representation of the loaded file
	* @param Filename name of the file to load
	* @param VerifyFlags flags controlling the hash verification behavior ( see EHashOptions )
	*/
	static bool LoadFileToString(FString& Result, const TCHAR* Filename, uint32 VerifyFlags = 0);

	/**
	* Save a binary array to a file.
	*/
	static bool SaveArrayToFile(TArrayView<const uint8> Array, const TCHAR* Filename, IFileManager* FileManager = &IFileManager::Get(), uint32 WriteFlags = 0);

	/**
	* Write the FString to a file.
	* Supports all combination of ANSI/Unicode files and platforms.
	*/
	static bool SaveStringToFile(const FString& String, const TCHAR* Filename, FFileHelper::EEncodingOptions EncodingOptions = FFileHelper::EEncodingOptions::AutoDetect, IFileManager* FileManager = &IFileManager::Get(), uint32 WriteFlags = 0);
	
	/**
	* 缓存目录.
	* 缓存目录.
	*/
	static FString ProjectPersistentDownloadDir();
    static FString ProjectHotloadDownloadDir();
};
