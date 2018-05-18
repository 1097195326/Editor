#include "OSSModule.h"
#include "Engine.h"
#include "IOSSDownload.h"
#include <iostream>
#include <fstream>
#include "HAL/PlatformFilemanager.h"
#include "GPlatformFileManager.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#elif PLATFORM_IOS
#include <Foundation/Foundation.h>
#endif
DEFINE_LOG_CATEGORY(LogOSS);
bool IOSSDownload::Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket)
{
	 AosEndPoint=_AosEndPoint;
	 AosAccessKeyID=_AosAccessKeyID;
	 AosAccessKeySecret=_AosAccessKeySecret;
	 AosBucket=_AosBucket;
	 m_isInit = true;
	 return true;
}


void IOSSDownload::Stop()
{

}

bool IOSSDownload::CanDownload() const
{
	return true;
}

bool IOSSDownload::CanUpload() const
{
	return true;
}
IOSSDownload::IOSSDownload()
{

}
IOSSDownload::~IOSSDownload()
{
}

void IOSSDownload::DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{
	PendingDownloadParameters = inParameters;

	OnFileDownloadProgress = inDelegate;
	OnFileDownloadFinish = inFinishDelegate;
}

void IOSSDownload::UploadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileUploadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{
	PendingUploadParameters = inParameters;

	OnFileUploadProgress = inDelegate;
	OnFileUploadFinish = inFinishDelegate;
}

void IOSSDownload::Download( FOssDownloadQueueParameter &inDownloadParameters)
{
	DownloadQueue.Add(inDownloadParameters);
	
}

void IOSSDownload::Upload( FOssUploadQueueParameter &inUpuloadParameters)
{
	UploadQueue.Add(inUpuloadParameters);
}


bool IOSSDownload::Tick(float DeltaTime)
{
	//文件下载.
	if (DownloadQueue.Num() > 0 && CanDownload())
	{
		TArray<FOssReqeustParameter> params;
		for (FOssReqeustParameter &elem : DownloadQueue[0].FilePaths)
		{
		
			params.Add(elem);
		}
		UE_LOG(LogTemp, Log, TEXT("Super Tick"));
		DownloadFiles(params, DownloadQueue[0].OnOSSFileDownloadProgress, DownloadQueue[0].OnOSSRequestFinished);
		DownloadQueue.RemoveAt(0);
	}

	// 文件上传.
	if (UploadQueue.Num() > 0 && CanUpload())
	{
		TArray<FOssReqeustParameter> params;
		for (FOssReqeustParameter &elem : UploadQueue[0].FilePaths)
		{
			params.Add(elem);
		}
		UploadFiles(params, UploadQueue[0].OnOSSFileUploadProgress, UploadQueue[0].OnOSSRequestFinished);
		UploadQueue.RemoveAt(0);
	}

	return true;
}

bool IOSSDownload::FileIsExist(const string &inFilePath)
{
	return GPlatformFileManager::FileExists(UTF8_TO_TCHAR(inFilePath.c_str()));
}

bool IOSSDownload::DirIsExist(const string &inDirPath)
{
	FString mAbsolutePath = FPaths::ConvertRelativePathToFull(inDirPath.c_str());
	UE_LOG(LogTemp, Log, TEXT("IOSS DirIsExist Dir : %s"), *mAbsolutePath);
	return GPlatformFileManager::DirectoryExists(*mAbsolutePath);
}

bool IOSSDownload::CreateDir(const string &inDirPath)
{
	//FString mTemp = inDirPath;
	FString mAbsolutePath = FPaths::ConvertRelativePathToFull(inDirPath.c_str());
	UE_LOG(LogTemp, Log, TEXT("IOS Create Dir : %s"), *mAbsolutePath);

	return GPlatformFileManager::RecursiveCreateDirectory(*mAbsolutePath);
}

string IOSSDownload::ExtractFileFolder(const string &inFilePath)
{
	return TCHAR_TO_UTF8(*GPlatformFileManager::ExtractFolder(UTF8_TO_TCHAR(inFilePath.c_str())));
}

std::string IOSSDownload::ExtractFileName(const string &inFilePath)
{
	return TCHAR_TO_UTF8(*GPlatformFileManager::ParentFolder(UTF8_TO_TCHAR(inFilePath.c_str())));
}


std::string IOSSDownload::FormatURLCode(const std::string& multiText)
{
	//std::string text;
	//ChineseCode::GB2312ToUTF_8(text, multiText.c_str(), multiText.length());
	//return text;
	return multiText;
}
