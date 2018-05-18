#pragma once
#include "IOSSDownload.h"
#if PLATFORM_IOS
#import "OSSClient.h"
#import "OSSModel.h"
#import "OSSTask.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface OSSDownloadUtils : NSObject
{
    OSSClient *m_ossClient;
    
    OSSPutObjectRequest * put;
    OSSTask * putTask;
    
    OSSGetObjectRequest * request;
    OSSTask * getTask;
}

+(id)sharedManager;

-(void)InitOSS:(NSString*) accessKeyId SecretKeyId:(NSString*) secretKeyId SecurityToken:(NSString*) securityToken HttpUrl:(NSString*) httpURL;
-(void)UploadFile:(NSString*) bucketName ObjectKey:(NSString*) objectKey UploadingFileURL:(NSString*)uploadingFileURL;
-(void)DownloadFile:(NSString*) bucketName ObjectKey:(NSString*) objectKey DownloadToFileURL:(NSString*)downloadToFileURL;

-(void)WaitPutTask;
-(void)WaitDownloadTask;


@end


using namespace std;
class FiOSOSSDownload:public IOSSDownload
{
private:
    // 是否上传下载成功.
    bool   bIsUploadFinish;
	int32  UploadProgressCurrent;
	int32  UploadProgressTotal;
	FString UploadFileName;
	bool bIsSingleUploading;
	// 上传下载进度.
    bool   bIsDownloadFinish;
	int32  DownloadCurrentSize;
	int32  DownloadTotalSize;
	FString DownloadFileName;
	bool bIsSingleDownloading;
public:
	FiOSOSSDownload();
	virtual ~FiOSOSSDownload();
	static  FiOSOSSDownload* Runnable;
	static FiOSOSSDownload *Get();
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

	void UpdateSingleDownloadProgress(const FString &inFileName, int64 inCurSize, int64 inTotalSize);
	void FinishSingleDownload(const FString &inFileName,bool success = true);
	void UpdateSingleUploadProgress(const FString &inFileName, int64 inCurSize, int64 inTotalSize);
	void FinishSingleUpload(const FString &inFileName);
};
#endif
