// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#if PLATFORM_WINDOWS || PLATFORM_MAC
#pragma once
#include "IOSSDownload.h"
#include <vector>
#include <thread>
#include <mutex>
#include <oss_define.h>
using namespace std;

class FOSSDownload:public IOSSDownload
{
public:
    // 上传下载进度.
	int32 m_downloadProgressCurrent;
	int32 m_downloadProgressTotal;
	int32 m_uploadProgressCurrent;
	int32 m_uploadProgressTotal;

	string mAosEndPoint;
	string mAosAccessKeyID;
	string mAosAccessKeySecret;
    
    // 接受下载完成队列.
    vector<string>    m_received_downloaded_queue;
    
    // 接受上传完成队列.
    vector<string>    m_received_uploaded_queue;
    
    mutex          m_msg_received_mutex;

	std::thread *  m_thread;
    
private:
    bool   m_isThreadBreak = false;
    
    // 是否上传下载成功.
    bool   m_isUploadFinish = false;
    bool   m_isDownloadFinish = false;
	bool	m_isFail = false;

	// windows 特殊处理stop.
	bool  m_canStop = false;
    
public:
	FOSSDownload();
	virtual ~FOSSDownload();
	static  FOSSDownload* Runnable;
	static FOSSDownload *Get();
public:
	// Begin FRunnable interface.
	virtual bool Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket) override;
	virtual bool Tick(float DeltaTime) override;
    uint32 DonwnloadOrUploadInThread();
	virtual void Stop() override;
	//single thread download.
	virtual bool CanDownload() const override;
	virtual bool CanUpload() const override;
    
	virtual void GRelease() override;

	virtual void ExitGame() override;
	
private:
    void InnerDownloadProcess(const TArray<FOssReqeustParameter> &inParameters);
    void InnerUploadProcess(const TArray<FOssReqeustParameter> &inParameters);
   
private:
    aos_pool_t *AosPool;
    oss_request_options_t *AosOptions;

	
public:
	virtual void DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate) override;
	virtual void UploadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileUploadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate) override;
};

#endif
