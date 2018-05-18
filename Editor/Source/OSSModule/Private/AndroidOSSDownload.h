#pragma once
#include "IOSSDownload.h"
#include <mutex>
using namespace std;

class FAndroidOSSDownload:public IOSSDownload
{
private:
    // 是否上传下载成功.
    bool   bIsUploadFinish;
	int32  UploadProgressCurrent;
	int32  UploadProgressTotal;
	FString UploadFileName;
	int32 CurrentUploadIndex;
	TArray<FOssReqeustParameter> TotalDownloadFiles;
	TArray<FOssReqeustParameter> DownloadFailedFiles;
	bool bIsSingleUploading;
	bool bIsPendindFinishDownload;
	// 上传下载进度.
    bool   bIsDownloadFinish;
	int32  DownloadProgressCurrent;
	int32  DownloadProgressTotal;
	FString DownloadFileName;
	int32 CurrentDownloadIndex;
	bool bIsSingleDownloading;
	bool bIsPendindFinishUpload;

	mutex  DownloadFinishMutex;
	mutex  DownloadProgressMutex;

public:
	FAndroidOSSDownload();
	virtual ~FAndroidOSSDownload();
	static  FAndroidOSSDownload* Runnable;
	static FAndroidOSSDownload *Get();
public:
	// Begin FRunnable interface.
	virtual bool Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket) override;
    virtual bool Tick(float DeltaTime) override;
	virtual void Stop() override;
	//single thread download.
	virtual bool CanDownload() const override;
	virtual bool CanUpload() const override;

	virtual void GRelease() override;

	virtual void ExitGame() override;

private:
    void InnerDownloadProcess(const FOssReqeustParameter &inParameter);
    void InnerUploadProcess(const FOssReqeustParameter &inParameter);

public:
	virtual void DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate) override;
	virtual void UploadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileUploadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate) override;

	void UpdateSingleDownloadProgress(int32 inFlag, int32 inCurSize, int32 inTotalSize);
	void FinishSingleDownload(int32 inFlag,bool success = true);
	void DownloadFailedHandle(int32 inFlag);
	void UpdateSingleUploadProgress(int32 inFlag, int32 inCurSize, int32 inTotalSize);
	void FinishSingleUpload(int32 inFlag);
	void UploadFailedHandle(int32 inFlag);
};
