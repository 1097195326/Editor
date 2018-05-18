// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Blueprint/UserWidget.h"
#include "AsyncTaskDownloadImage.h"
#include "UObject/ObjectMacros.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include <thread>
#include <mutex>
#include <iostream>
#include "ILoad.h"
#include <stdint.h>
#include "IUpdate.h"
#include <string>
#include "castle_std.h"
#include "DownloadImageTool.generated.h"

enum EnumDownloadImage
{
	WAITTING_DOWNLOAD = 0,
	DOWNLOADING,
	DOWNLOAD_FINISH
};

struct DownloadImageParam
{
	FString m_downloadUrl;

	FString m_savePath;

	EnumDownloadImage m_isDownloadFinish = EnumDownloadImage::WAITTING_DOWNLOAD;

	DownloadImageParam()
	{
	}

	DownloadImageParam(FString _downloadUrl, FString _savePath)
	{
		m_downloadUrl = _downloadUrl;

		m_savePath = _savePath;
	}
};

struct DownloadImageStruct
{
	TArray<DownloadImageParam*> m_params;

	std::function<void()> m_callbackFunc;

	template<typename T>
	DownloadImageStruct(TArray<DownloadImageParam*> & _params,T* obj, void(T::*_func)())
	{
		m_params = _params;

		m_callbackFunc = std::function<void()>(std::bind(_func, obj));
	}
};


/**
 * 
 */
UCLASS()
class GCORE_API UDownloadImageTool : public UBlueprintAsyncActionBase,public ILoad,public IUpdate
{
	GENERATED_UCLASS_BODY()

private:
	// 设置单利.
	static UDownloadImageTool * m_instance;

	// 下载队列.
	TArray<DownloadImageStruct*> m_downloadQuene;

	// 当前下载的图片对象.
	DownloadImageParam * m_currentDownloadParam;
    
    FString m_downloadUrl;

	int32 m_repeatCount = 0;

	int32 m_maxRepeatCount = 3;

public:

	FString   m_url;

	int32     m_penetrateId = 0;

	bool      m_downloadFinish = false;

	bool      m_destroy = false;

	TArray<TSharedRef<IHttpRequest>> m_HttpRequest;
	
	std::function<void(bool _isScuccess,int32 _penetrateId, FString _url,UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)> m_callbackFunc;

public:

	static UDownloadImageTool * GetInstance();
	
	template<typename T>
	static UDownloadImageTool* DownloadImage(int32 _penetrateId,const FString & _url, T* obj, void (T::*_func)(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool))
	{
		UDownloadImageTool * mDownloadImage = NewObject<UDownloadImageTool>();

		mDownloadImage->m_callbackFunc = std::function<void(bool _isScuccess, int32 _penetrateId, FString _url, UTexture2D * _texture2d, UDownloadImageTool * _downloadTool)>(std::bind(_func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

		mDownloadImage->m_url = _url;

		mDownloadImage->m_penetrateId = _penetrateId;

		mDownloadImage->StartDownload();

		return mDownloadImage;
	}

	static UTexture2D * GetTexture2dByUnit8Array(const TArray<uint8> & _array, int32 _lenth);

public:

	void StartDownload();

	void CancelDownload();

	virtual void GLoad() override;

	virtual void GRelease() override;

	virtual void OnUpdate(float DeltaTime) override;

	void DownloadImage(DownloadImageStruct * _downloadStruct);

public:
	void StartDownloadImage(FString _imageUrl);

	void DownloadFinish(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess);

	void DownloadImageFinish(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess);
};
