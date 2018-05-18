//
//  HotLoadManager.hpp
//  Editor
//
//  Created by hongxing zhang on 2018/2/23.
//  Copyright © 2018年 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "Engine.h"
#include "XmlParser.h"
#include "tinyxml.h"
#include "tinystr.h"
#include <queue>
#include "OSSHelper.h"

#include "HotLoadManager.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVersionCheckFinished, bool, isSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVersionFileDownloadProgress, FString, inFileName, float, inProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVersionFileDownloadTotalProgress, FString, inProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVersionFileDownloadSpeed, FString, inProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVersionFileDownloadTotalSize, int, inSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnVersionInfoFinished, FString, msg, FString, url, int, forced);

USTRUCT(BlueprintType)
struct FFileInfo
{
	GENERATED_USTRUCT_BODY()
public:
	bool operator ==(const FFileInfo& other) {
		return other.Name == this->Name && other.MD5 == this->MD5 && other.Size == this->Size;
	}
	FString Name;
	FString MD5;
	int Size;
};
enum class FileState : int8
{
	F_Waiting = 1,
	F_Progressing,
	F_Finished,
	F_Fail,
};
struct DownLoadFile
{
	DownLoadFile() :m_State(FileState::F_Waiting) {}
	FString		m_xmlName;
	TArray<FOssReqeustParameter> m_Parameters;
	FOnOSSFileDownloadProgressDelegate m_ProgressDelegate;
	FOnOSSRequestFinishedDelegate m_FinishedDelegate;
	FileState m_State;
};


UCLASS(BlueprintType)
class UHotLoadManager : public UObject
{
	GENERATED_BODY()
private:
	UHotLoadManager();

private:
	float CreationTime;

	std::queue<DownLoadFile *> m_AllTask;
	static UHotLoadManager * Manager;

	bool        m_isLoading;
	DownLoadFile *    m_CurrentLoadingFile;
	FTimerHandle  m_LoadTimerHandler;

	int            m_TotalSize;
	float          m_CurrentSize;
public:
	~UHotLoadManager();
	static UHotLoadManager * GetInstance();
	void Init();
	void Destroy();
	void Tick();
	float GetGameTimeSinceCreation();


	void DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate, FString xmlName = TEXT(""));
	void OnDownloadFileFinish(bool res = true);
	void OnDownloadFileProgress(FString inFileName, int32 inCurrentSize, int32 inTotalSize);
public:
	void PakMount();
	// Version configure file
	void GetVersionXML();
	void OnCBGetVersionXML(bool res = true);
	void OnCBGetVersionXMLProgress(FString inFileName, int32 inCurrentSize, int32 inTotalSize);
	//download version File
	void DownloadVersionFiles(const TArray<FString> &inFiles);
	void OnCBDownloadVersionFiles(bool res = true);
	void OnCBDownloadVersionFileProgress(FString inFileName, int32 inCurrentSize, int32 inTotalSize);

	// 自动检查是否下载完成.
	bool IsDownloadFinish();
	bool CheckDownloadFileSucess();
	bool CheckDownloadAllSucess(TArray<FString> & arr);
	void CheckFinished(bool inSuccess);


	void GetUpdateFiles(TArray< FString >& arr);

	//强制更新..

	void VersionForcedUpdateCheck();

	void OpenURL(FString inURL);

	FOnVersionInfoFinished  OnVersionInfoFinished;

protected:

	FOnVersionCheckFinished OnVersionCheckFinished;


	FOnVersionFileDownloadProgress OnVersionDownloadProgress;


	FOnVersionFileDownloadTotalProgress OnVersionDownloadTotalProgress;


	FOnVersionFileDownloadSpeed onVersionFileDownloadSpeed;


	FOnVersionFileDownloadTotalSize onVersionFileDownloadTotalSize;

	void GetDiffVersions(const FString& oldfile, const FString& newfile, TArray< FString >& arr);
	TMap < FString, FFileInfo> GetDiffNewFromMapInfo(const TMap<FString, FFileInfo>& oldsrc, const TMap<FString, FFileInfo>& newsrc);
	TMap < FString, FFileInfo> GetFileMapFromXml(TiXmlElement* xmlobj);


	bool DownloadCheckPak();
	TArray<FString> GetFileNameFromXml(TiXmlElement * xmlobj);
	FString GetLocalVersionFilePath(bool IsExtract = false);
	FString GetNetVersionFilePath(bool IsExtract = false);

	FString GetVersionXmlFilePath(FString fileName);
private:
	//版本文件
	TMap<FString, FString>	m_loadFileMD;


	FString VersionNetXmlFile;
	FString VersionLocalXmlFile;
	FString VersionUrlRootDir;
	FString VersionLocalRootDir;
	TArray<FString> PakFiles;
	TArray<FString> m_DownloadedFiles;

	int32 CurrentDownloadNumUnbelive;
	int32 LastDownloadSize;
	float LastCalTime;

	bool VersionXMLDownloadFinishFlag;

	bool VersionPakDownloadFinishFlag;

};
