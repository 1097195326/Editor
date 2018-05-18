// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OSSModule.h"
#include <string>
#include "Containers/Ticker.h"
#include <functional>


DECLARE_LOG_CATEGORY_EXTERN(LogOSS, Log, All);

using namespace std;
typedef std::function<void(FString, int32, int32)> FOnOSSFileDownloadProgress;//FString:filename int32:currentsize  int32:totalsize
typedef std::function<void(FString, int32, int32)> FOnOSSFileUploadProgress;
typedef std::function<void(bool)> FOnOSSRequestFinished;

//请求oss文件路径参数.
struct FOssReqeustParameter
{
	FString OssPath;   // 网络地址.
	FString LocalPath; // 本地存放文件名.
	FOssReqeustParameter(const FString&inOssPath, const FString &inLocalPath)
	{
		OssPath = inOssPath;
		LocalPath = inLocalPath;
	}
};

//下载代理.
struct FOnOSSFileDownloadProgressDelegate
{
public:
	FOnOSSFileDownloadProgressDelegate() {}
	template<typename T>
	FOnOSSFileDownloadProgressDelegate(T* obj, void(T::*_func)(FString, int32, int32))
	{
		OnOSSFileDownloadProgress = FOnOSSFileDownloadProgress(std::bind(_func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	void Execute(FString fileName, int32 currentSize, int32 totalSize)
	{
		OnOSSFileDownloadProgress(fileName, currentSize, totalSize);
	}
private:
	FOnOSSFileDownloadProgress OnOSSFileDownloadProgress;
};

//上传代理.
struct FOnOSSFileUploadProgressDelegate
{

public:
	FOnOSSFileUploadProgressDelegate() {}
	template<typename T>
	FOnOSSFileUploadProgressDelegate(T* obj, void(T::*_func)(FString, int32, int32))
	{
		OnOSSFileUploadProgress = FOnOSSFileUploadProgress(std::bind(_func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	void Execute(FString fileName, int32 currentSize, int32 totalSize)
	{
		OnOSSFileUploadProgress(fileName, currentSize, totalSize);
	}
private:
	FOnOSSFileUploadProgress OnOSSFileUploadProgress;
};

//完成代理.
struct FOnOSSRequestFinishedDelegate
{
public:
	FOnOSSRequestFinishedDelegate() {}
	template<typename T>
	FOnOSSRequestFinishedDelegate(T *obj, void(T::*_func)(bool))
	{
		OnOSSRequestFinished = FOnOSSRequestFinished(std::bind(_func, obj,std::placeholders::_1));
	}
	void Execute(bool res = true)
	{
		OnOSSRequestFinished(res);
	}
private:
	FOnOSSRequestFinished OnOSSRequestFinished;
};

//上传队列结构体.
struct FOssDownloadQueueParameter
{
	TArray<FOssReqeustParameter> FilePaths;
	FOnOSSFileDownloadProgressDelegate OnOSSFileDownloadProgress;
	FOnOSSRequestFinishedDelegate OnOSSRequestFinished;
};

//下载队列结构体.
struct FOssUploadQueueParameter
{
	TArray<FOssReqeustParameter> FilePaths;
	FOnOSSFileUploadProgressDelegate OnOSSFileUploadProgress;
	FOnOSSRequestFinishedDelegate OnOSSRequestFinished;
};
class OSSMODULE_API IOSSDownload
{
public:
	IOSSDownload();
	virtual ~IOSSDownload();

protected:
	bool m_isInit = false;
public:
	// Begin FRunnable interface.
	virtual bool Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket);
	virtual void Stop();
	//single thread download.
	virtual bool CanDownload() const;
	virtual bool CanUpload() const;

	virtual void GRelease() = 0;

	virtual void ExitGame() = 0;

	void Download( FOssDownloadQueueParameter &inDownloadParameters);
	void Upload( FOssUploadQueueParameter &inUpuloadParameters);
	virtual bool Tick(float DeltaTime);
public:
	virtual void DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate);
	virtual void UploadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileUploadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate);
public:

	bool FileIsExist(const string &inFilePath);
	bool DirIsExist(const string &inDirPath);
	bool CreateDir(const string &inDirPath);
	string ExtractFileFolder(const string &inFilePath);
	string ExtractFileName(const string &inFilePath);
	string FormatURLCode(const std::string& multiText);
public:
    FOnOSSFileDownloadProgressDelegate OnFileDownloadProgress;
    FOnOSSRequestFinishedDelegate OnFileDownloadFinish;
    FOnOSSFileUploadProgressDelegate OnFileUploadProgress;
    FOnOSSRequestFinishedDelegate OnFileUploadFinish;


    
public:
    //即将下载的文件.
    TArray<FOssReqeustParameter> PendingDownloadParameters;

    // 即将上传的文件.
	TArray<FOssReqeustParameter> PendingUploadParameters;

	//下载的文件队列.
	TArray<FOssDownloadQueueParameter> DownloadQueue;

	// 上传的文件队列.
	TArray<FOssUploadQueueParameter> UploadQueue;
    int32 MaxBatchNum;
	TArray<FOssReqeustParameter> DownloadErrorParameters;
	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;
	FString AosEndPoint;
	FString AosAccessKeyID;
	FString AosAccessKeySecret;
	FString AosBucket;
};
