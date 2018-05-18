#include "OSSModule.h"
#include "iOSOSSDownload.h"
#if PLATFORM_IOS
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/Mac/CocoaThread.h"
#include "Engine/Engine.h"
using namespace std;

// ============================ ios ============================


@implementation OSSDownloadUtils

// 单例.
static OSSDownloadUtils *_instance;

+ (instancetype)allocWithZone:(struct _NSZone *)zone
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [super allocWithZone:zone];
    });
    return _instance;
}

+ (id)sharedManager
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[[self alloc] init] retain];
    });
    return _instance;
}

- (id)copyWithZone:(NSZone *)zone
{
    return _instance;
}

// 初始化OSS.
-(void)InitOSS:(NSString*) accessKeyId SecretKeyId:(NSString*) secretKeyId SecurityToken:(NSString*) securityToken HttpUrl:(NSString*) httpURL;
{
    id<OSSCredentialProvider> credential = [[OSSPlainTextAKSKPairCredentialProvider alloc] initWithPlainTextAccessKey:accessKeyId secretKey:secretKeyId];
    m_ossClient = [[[OSSClient alloc] initWithEndpoint:httpURL credentialProvider:credential] retain];
}

// 上传文件.
-(void)UploadFile:(NSString*) bucketName ObjectKey:(NSString*) objectKey UploadingFileURL:(NSString*)uploadingFileURL
{
    put = [OSSPutObjectRequest new];
    // 必填字段.
    put.bucketName = bucketName;
    put.objectKey = objectKey;
    put.uploadingFileURL = [NSURL fileURLWithPath:uploadingFileURL];
    // put.uploadingData = <NSData *>; // 直接上传NSData
    // 可选字段，可不设置.
    put.uploadProgress = ^(int64_t bytesSent, int64_t totalByteSent, int64_t totalBytesExpectedToSend) {
        // 当前上传段长度、当前已经上传总长度、一共需要上传的总长度.
        //NSLog(@"%lld, %lld, %lld", bytesSent, totalByteSent, totalBytesExpectedToSend);
        
        FiOSOSSDownload::Get()->UpdateSingleUploadProgress(FString([uploadingFileURL UTF8String]),totalByteSent,totalBytesExpectedToSend);
    };
    // 以下可选字段的含义参考： https://docs.aliyun.com/#/pub/oss/api-reference/object&PutObject.
    
    putTask = [m_ossClient putObject:put];
    [putTask continueWithBlock:^id(OSSTask *task) {
        if (!task.error) {
            FString mDownloadPath = ([uploadingFileURL UTF8String]);
            //GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT(" 上传成功 %s"), *mDownloadPath));
            FiOSOSSDownload::Get()->FinishSingleUpload(FString([uploadingFileURL UTF8String]));
        } else {
            NSString *strError = [NSString stringWithFormat:@"%@",task.error];
            FString mError = ([strError UTF8String]);
            //            FString mDownloadPath = ([uploadingFileURL UTF8String]);
            
            UE_LOG(LogOSS, Error, TEXT("UPLOAD FAILE %s"), *mError);
            
            //            FString left = mDownloadPath.Left(mDownloadPath.Len() / 2);
            //            mDownloadPath.RemoveAt(0,mDownloadPath.Len() / 2);
            //            FString right = mDownloadPath;
            //            mDownloadPath = FString::Printf(TEXT("%s\n%s"), *left, *right);
            //
            //GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT(" 上传失败 %s"), *mDownloadPath));
            //GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT(" 上传失败 %s"), *mError));
            FiOSOSSDownload::Get()->FinishSingleUpload(FString([uploadingFileURL UTF8String]));
        }
        return nil;
    }];
}

-(void)WaitPutTask
{
    [putTask waitUntilFinished];
    [put cancel];
}
-(void)WaitDownloadTask
{
    [getTask waitUntilFinished];
    [request cancel];
}

// 下载文件.
-(void)DownloadFile:(NSString*) bucketName ObjectKey:(NSString*) objectKey DownloadToFileURL:(NSString*)downloadToFileURL
{
    request = [OSSGetObjectRequest new];
    // 必填字段.
    request.bucketName = bucketName;
    request.objectKey = objectKey;
    // 可选字段.
    request.downloadProgress = ^(int64_t bytesWritten, int64_t totalBytesWritten, int64_t totalBytesExpectedToWrite) {
        // 当前下载段长度、当前已经下载总长度、一共需要下载的总长度.
        FiOSOSSDownload::Get()->UpdateSingleDownloadProgress(FString([downloadToFileURL UTF8String]),totalBytesWritten,totalBytesExpectedToWrite);
    };
    
    request.downloadToFileURL = [NSURL fileURLWithPath:downloadToFileURL]; // 如果需要直接下载到文件，需要指明目标文件地址.
    getTask = [m_ossClient getObject:request];
    [getTask continueWithBlock:^id(OSSTask *task) {
        if (!task.error) {
            
            //NSLog(@"download object success!");
            FString mDownloadPath = ([downloadToFileURL UTF8String]);
            FiOSOSSDownload::Get()->FinishSingleDownload(mDownloadPath);
            
        } else {
            
            FString mDownloadPath = ([downloadToFileURL UTF8String]);
            //            FString mObjKey = ([objectKey UTF8String]);
            //            FString mBucketName = ([bucketName UTF8String]);
            //            GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT(" 下载失败 %s"), *mDownloadPath));
            //            GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("objkey %s"), *mObjKey));
            //            GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("mBucketName %s"), *mBucketName));
            FiOSOSSDownload::Get()->FinishSingleDownload(mDownloadPath,false);
            NSString *strError = [[NSString alloc] initWithFormat:@"%@",task.error];
            const char * charError =[strError UTF8String];
            FString mErrorString(charError);
            UE_LOG(LogOSS, Error, TEXT("DOWANLOAD FAILE %s"), *mErrorString);
        }
        return nil;
    }];
}

@end

// ==============================================================


FiOSOSSDownload* FiOSOSSDownload::Runnable = nullptr;

FiOSOSSDownload::FiOSOSSDownload() :
DownloadCurrentSize(0),
DownloadTotalSize(0),
bIsDownloadFinish(false),
bIsSingleDownloading(false),
UploadProgressCurrent(0),
UploadProgressTotal(0),
bIsUploadFinish(false),
bIsSingleUploading(false)
{
    
}

FiOSOSSDownload::~FiOSOSSDownload()
{
}

FiOSOSSDownload * FiOSOSSDownload::Get()
{
    if (FiOSOSSDownload::Runnable == nullptr)
    {
        Runnable = new FiOSOSSDownload();
    }
    return Runnable;
}


bool FiOSOSSDownload::Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket)
{
    IOSSDownload::Init( _AosEndPoint,  _AosAccessKeyID,  _AosAccessKeySecret,  _AosBucket);
#if PLATFORM_IOS
    UE_LOG(LogOSS, Log, TEXT("INITIALISE OSS DOWNLOAD"));
    
    NSString *endpoint = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*AosEndPoint)];
    // 由阿里云颁发的AccessKeyId/AccessKeySecret构造一个CredentialProvider.
    NSString *accessKeyId = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*AosAccessKeyID)];
    NSString *secretKeyId = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*AosAccessKeySecret)];
    NSString *securityToken = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*AosBucket)];
    
    // 初始化oss.
    [[OSSDownloadUtils sharedManager]InitOSS:accessKeyId SecretKeyId:secretKeyId SecurityToken:securityToken HttpUrl:endpoint];
    
#endif
    TickDelegate = FTickerDelegate::CreateRaw(this, &FiOSOSSDownload::Tick);
    TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
    return false;
}

void FiOSOSSDownload::Stop()
{
    
}

bool FiOSOSSDownload::CanDownload() const
{
    return PendingDownloadParameters.Num() == 0;
}

bool FiOSOSSDownload::CanUpload() const
{
    return PendingUploadParameters.Num() == 0;
}


void FiOSOSSDownload::GRelease()
{
	Stop();

    delete Runnable;
    
    Runnable = nullptr;
}


void FiOSOSSDownload::ExitGame()
{
	Stop();

	delete Runnable;

	Runnable = nullptr;
}

void FiOSOSSDownload::InnerDownloadProcess(const FOssReqeustParameter &inParameter)
{
#if PLATFORM_IOS
    
    FString ossPath = FString::Printf(TEXT("GameResource/%s"), *inParameter.OssPath);
    const string utfLocalPath(TCHAR_TO_UTF8(*inParameter.LocalPath));
    const string utfOSSPath(TCHAR_TO_UTF8(*ossPath));
    
    if (!DirIsExist(ExtractFileFolder(utfLocalPath)) && !CreateDir(ExtractFileFolder(utfLocalPath)))
    {
        
        UE_LOG(LogOSS, Error, TEXT("Cannot create dir %s"), *inParameter.LocalPath);
        return;
    }
    string urlStr = FormatURLCode(utfOSSPath);
    string localStr = FormatURLCode(utfLocalPath);
    string mBucket(TCHAR_TO_UTF8(*AosBucket));
    NSString *bucketName = [NSString stringWithFormat:@"%s",mBucket.c_str()];
    NSString *objectKey =  [NSString stringWithFormat:@"%s",urlStr.c_str()];
    NSString *downloadToFileURL = [NSString stringWithFormat:@"%s",localStr.c_str()];
    
    bIsSingleDownloading = true;
    
    //通知主线程刷新.
    [[OSSDownloadUtils sharedManager] DownloadFile:bucketName ObjectKey:objectKey DownloadToFileURL:downloadToFileURL];
    
#endif
}

void FiOSOSSDownload::InnerUploadProcess(const FOssReqeustParameter &inParameter)
{
#if PLATFORM_IOS
    FString ossPath = FString::Printf(TEXT("GameResource/%s"), *inParameter.OssPath);
    const string utfLocalPath(TCHAR_TO_UTF8(*inParameter.LocalPath));
    const string utfOSSPath(TCHAR_TO_UTF8(*ossPath));
    if (!DirIsExist(ExtractFileFolder(utfLocalPath)) && !CreateDir(ExtractFileFolder(utfLocalPath)))
    {
        UE_LOG(LogOSS, Error, TEXT("Cannot create dir %s"), *inParameter.LocalPath);
        return;
    }
    
    string urlStr = FormatURLCode(utfOSSPath);
    string localStr = FormatURLCode(utfLocalPath);
    
    // 必填字段.
    string mBucket(TCHAR_TO_UTF8(*AosBucket));
    NSString *bucketName = [NSString stringWithFormat:@"%s",mBucket.c_str()];
    NSString *objectKey =  [NSString stringWithFormat:@"%s",urlStr.c_str()];
    NSString *uploadingFileURL = [NSString stringWithFormat:@"%s",localStr.c_str()];
    
    bIsSingleUploading = true;
    
    [[OSSDownloadUtils sharedManager] UploadFile:bucketName ObjectKey:objectKey UploadingFileURL:uploadingFileURL];
#endif
}

bool FiOSOSSDownload::Tick(float DeltaTime)
{
    IOSSDownload::Tick(DeltaTime);
    //进度
    if (bIsSingleDownloading)
    {
        OnFileDownloadProgress.Execute(FPaths::GetBaseFilename(DownloadFileName), DownloadCurrentSize, DownloadTotalSize == 0 ? INT32_MAX : DownloadTotalSize);
        
        if(DownloadTotalSize != 0 && DownloadCurrentSize >= DownloadTotalSize)
        {
            // 表示下载完成一个.
            bIsSingleDownloading = false;
            
            DownloadCurrentSize = 0;
            
            DownloadTotalSize = 0;
            
            if(PendingDownloadParameters.Num()==0)
            {
                bIsDownloadFinish = true;
            }
            
            [[OSSDownloadUtils sharedManager] WaitDownloadTask];
        }
        return true;
    }
    
    if (bIsSingleUploading)
    {
        OnFileUploadProgress.Execute(FPaths::GetBaseFilename(UploadFileName), UploadProgressCurrent, UploadProgressTotal == 0 ? INT32_MAX : UploadProgressTotal);
        
        if(UploadProgressCurrent >= UploadProgressTotal)
        {
            // 表示下载完成一个.
            bIsSingleUploading = false;
            
            UploadProgressCurrent = 1;
            
            UploadProgressTotal = 1;
            
            if(PendingUploadParameters.Num()==0)
            {
                bIsUploadFinish = true;
            }
            
            [[OSSDownloadUtils sharedManager] WaitPutTask];
        }
        return true;
    }
    
    //完成
    if (bIsDownloadFinish)
    {
        bIsDownloadFinish = false;
        UE_LOG(LogOSS, Log,TEXT("===下载成功,发送通知=="));
        OnFileDownloadFinish.Execute();
        //OnFileDownloadFinish.Unbind();
        return true;
    }
    if (bIsUploadFinish)
    {
        bIsUploadFinish = false;
        OnFileUploadFinish.Execute();
        //OnFileUploadFinish.Unbind();
        return true;
    }
    
    
    if (PendingDownloadParameters.Num() > 0 && !bIsSingleDownloading)
    {
        bIsSingleDownloading = true;
        InnerDownloadProcess(PendingDownloadParameters[PendingDownloadParameters.Num() - 1]);
        PendingDownloadParameters.RemoveAt(PendingDownloadParameters.Num() - 1);
    }
    
    if (PendingUploadParameters.Num() > 0 && !bIsSingleUploading)
    {
        bIsSingleUploading = true;
        InnerUploadProcess(PendingUploadParameters[PendingUploadParameters.Num() - 1]);
        PendingUploadParameters.RemoveAt(PendingUploadParameters.Num() - 1);
    }
    
    return true;
}


void FiOSOSSDownload::DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{
    UE_LOG(LogOSS, Log, TEXT("NEW PENDING DOWNLOAD REQUEST..."));
    
    
    PendingDownloadParameters = inParameters;
    OnFileDownloadProgress = inDelegate;
    OnFileDownloadFinish = inFinishDelegate;
}

void FiOSOSSDownload::UploadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileUploadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{
    PendingUploadParameters = inParameters;
    OnFileUploadProgress = inDelegate;
    OnFileUploadFinish = inFinishDelegate;
    //GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, TEXT("开始下载"));
}

void FiOSOSSDownload::UpdateSingleDownloadProgress(const FString &inFileName, int64 inCurSize, int64 inTotalSize)
{
//    if (!bIsSingleDownloading)
//    {
//        return;
//    }
    DownloadFileName = inFileName;
    DownloadCurrentSize = inCurSize;
    DownloadTotalSize = inTotalSize;
    
    if(DownloadCurrentSize == DownloadTotalSize)
    {
        if(DownloadTotalSize == 0)
            DownloadTotalSize = 1;
        DownloadCurrentSize = DownloadCurrentSize-1;
    }
    
    UE_LOG(LogOSS, Log,TEXT("UpdateSingleDownloadProgress (%s) %lu/%lu"), *inFileName, inCurSize, inTotalSize);
}

void FiOSOSSDownload::FinishSingleDownload(const FString &inFileName,bool success)
{
    UE_LOG(LogOSS, Log,TEXT("FinishSingleDownload (%s)"), *inFileName);
    if(success)
    {
        DownloadTotalSize = DownloadTotalSize == 0 ? INT32_MAX : DownloadTotalSize;
        
        DownloadCurrentSize = DownloadTotalSize;
        
        DownloadFileName = inFileName;
    }else
    {
        bIsSingleDownloading = false;
        bIsDownloadFinish = false;
        PendingDownloadParameters.Empty();
        OnFileDownloadFinish.Execute(false);
    }
}

void FiOSOSSDownload::UpdateSingleUploadProgress(const FString &inFileName, int64 inCurSize, int64 inTotalSize)
{
    if (!bIsSingleUploading)
    {
        return;
    }
    UploadFileName = inFileName;
    UploadProgressCurrent = inCurSize;
    UploadProgressTotal = inTotalSize;
    UE_LOG(LogOSS, Log,TEXT("UPLOAD (%s) %lu/%lu"),*inFileName,inCurSize,inTotalSize);
}

void FiOSOSSDownload::FinishSingleUpload(const FString &inFileName)
{
    
    UploadProgressTotal = UploadProgressTotal == 0 ? INT32_MAX : UploadProgressTotal;
    
    UploadProgressCurrent = UploadProgressTotal;
    
    UploadFileName = inFileName;
    
    UE_LOG(LogOSS, Log, TEXT("Finish Upload file %s"), *inFileName);
}

#endif



