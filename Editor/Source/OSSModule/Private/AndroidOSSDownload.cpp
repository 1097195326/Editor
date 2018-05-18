#include "OSSModule.h"
#include "AndroidOSSDownload.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif

using namespace std;
#if PLATFORM_ANDROID
extern "C"
{
	// 下载
	void Java_com_oss_dachengshidai_OSSModuleNative_onDownloadProgress(JNIEnv* inEnv, jobject, jint inFlag, jlong inCurSize, jlong inTotalSize)
	{
		FAndroidOSSDownload::Get()->UpdateSingleDownloadProgress(inFlag, inCurSize, inTotalSize);
	}
	void Java_com_oss_dachengshidai_OSSModuleNative_onDownloadSuccess(JNIEnv *inEnv, jobject, jint inFlag)
	{
		FAndroidOSSDownload::Get()->FinishSingleDownload(inFlag);
	}
	void Java_com_oss_dachengshidai_OSSModuleNative_onDownloadFailed(JNIEnv *inEnv, jobject, jint inFlag, jint inErrorCode, jstring inReason)
	{
		const char* reason = inEnv->GetStringUTFChars(inReason, 0);
		if (inErrorCode == -1)//文件IO异常，尝试重新下载
		{
			FAndroidOSSDownload::Get()->DownloadFailedHandle(inFlag);
		}
		FAndroidOSSDownload::Get()->FinishSingleDownload(inFlag,false);
		UE_LOG(LogOSS, Log, TEXT("failed download file . errorcode %d, reason %s"), inErrorCode, UTF8_TO_TCHAR(reason));
		inEnv->ReleaseStringUTFChars(inReason, reason);
	}

	// 上传
	void Java_com_oss_dachengshidai_OSSModuleNative_onUploadProgress(JNIEnv* inEnv, jobject, jint inFlag, jlong inCurSize, jlong inTotalSize)
	{
		FAndroidOSSDownload::Get()->UpdateSingleUploadProgress(inFlag, inCurSize, inTotalSize);
	}

	void Java_com_oss_dachengshidai_OSSModuleNative_onUploadSuccess(JNIEnv *inEnv, jobject, jint inFlag)
	{
		FAndroidOSSDownload::Get()->FinishSingleUpload(inFlag);
	}

	void Java_com_oss_dachengshidai_OSSModuleNative_onUploadFailed(JNIEnv *inEnv, jobject, jint inFlag, jint inErrorCode, jstring inReason)
	{
		const char* reason = inEnv->GetStringUTFChars(inReason, 0);
		FAndroidOSSDownload::Get()->FinishSingleUpload(inFlag);
		UE_LOG(LogOSS, Log, TEXT("failed upload file ,errorcode %d, reason %s"), inErrorCode, UTF8_TO_TCHAR(reason));
		inEnv->ReleaseStringUTFChars(inReason, reason);
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString("Failed Upload file : ").Append(UTF8_TO_TCHAR(charsDesc))));
	}

	void Java_com_oss_dachengshidai_OSSModuleNative_onDirectoryCannotCreate(JNIEnv *inEnv, jobject, jstring inFileName)
	{
		const char* tag = inEnv->GetStringUTFChars(inFileName, 0);
		UE_LOG(LogOSS, Error, TEXT("can not create directory %s"), UTF8_TO_TCHAR(tag));
		inEnv->ReleaseStringUTFChars(inFileName, tag);
	}

	void Java_com_oss_dachengshidai_OSSModuleNative_OSSLog(JNIEnv *inEnv, jobject, jstring inTag, jstring inMsg)
	{
		const char* tag = inEnv->GetStringUTFChars(inTag, 0);
		const char* msg = inEnv->GetStringUTFChars(inMsg, 0);
		UE_LOG(LogOSS, Log, TEXT("%s : %s"), UTF8_TO_TCHAR(tag), UTF8_TO_TCHAR(msg));
		inEnv->ReleaseStringUTFChars(inTag, tag);
		inEnv->ReleaseStringUTFChars(inMsg, msg);
	}
}

#endif
FAndroidOSSDownload* FAndroidOSSDownload::Runnable = nullptr;

FAndroidOSSDownload::FAndroidOSSDownload() :
	DownloadProgressCurrent(0),
	DownloadProgressTotal(0),
	bIsDownloadFinish(false),
	bIsSingleDownloading(false),
	UploadProgressCurrent(0),
	UploadProgressTotal(0),
	bIsUploadFinish(false),
	bIsSingleUploading(false),
	CurrentDownloadIndex(-1),
	bIsPendindFinishDownload(false),
	bIsPendindFinishUpload(false)
{

}

FAndroidOSSDownload::~FAndroidOSSDownload()
{
}

FAndroidOSSDownload * FAndroidOSSDownload::Get()
{
	if (Runnable == nullptr)
	{
		Runnable = new FAndroidOSSDownload();
	}
	return Runnable;
}


bool FAndroidOSSDownload::Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket)
{
	IOSSDownload::Init(_AosEndPoint, _AosAccessKeyID, _AosAccessKeySecret, _AosBucket);
#if PLATFORM_ANDROID
	UE_LOG(LogOSS, Log, TEXT("INITIALISE OSS DOWNLOAD"));
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			static jmethodID Method = Env->GetStaticMethodID(Class, "AndroidOSS_InitOSS", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
			jstring ak = Env->NewStringUTF(TCHAR_TO_UTF8(*AosAccessKeyID));
			jstring as = Env->NewStringUTF(TCHAR_TO_UTF8(*AosAccessKeySecret));
			jstring ep = Env->NewStringUTF(TCHAR_TO_UTF8(*AosEndPoint));
			jstring bk = Env->NewStringUTF(TCHAR_TO_UTF8(*AosBucket));

			if (!Env->CallStaticBooleanMethod(Class, Method, ep, ak, as, bk))
			{
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Cannot initialise android oss sdk")));
			}
			else
			{
				UE_LOG(LogOSS, Log, TEXT("SUCCESS TO INITIALISE OSS DOWNLOAD"));
			}
			Env->DeleteLocalRef(ak);
			Env->DeleteLocalRef(as);
			Env->DeleteLocalRef(ep);
			Env->DeleteLocalRef(bk);
		}
		Env->DeleteLocalRef(Class);
	}
#endif
	TickDelegate = FTickerDelegate::CreateRaw(this, &FAndroidOSSDownload::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
	return false;
}

void FAndroidOSSDownload::Stop()
{
	
}

bool FAndroidOSSDownload::CanDownload() const
{
	return PendingDownloadParameters.Num() == 0 && !bIsSingleDownloading;
}

bool FAndroidOSSDownload::CanUpload() const
{
	return PendingUploadParameters.Num() == 0 && !bIsSingleUploading;
}


void FAndroidOSSDownload::GRelease()
{
	Stop();

	delete Runnable;

	Runnable = nullptr;
}


void FAndroidOSSDownload::ExitGame()
{
	Stop();

	delete Runnable;

	Runnable = nullptr;
}

void FAndroidOSSDownload::InnerDownloadProcess(const FOssReqeustParameter &inParameter)
{

#if PLATFORM_ANDROID
	FString ossPath = FString::Printf(TEXT("GameResource/%s"), *inParameter.OssPath);
	const string utfLocalPath(TCHAR_TO_UTF8(*inParameter.LocalPath));
	const string utfOSSPath(TCHAR_TO_UTF8(*ossPath));
	
	string urlStr = FormatURLCode(utfOSSPath);
	string localStr = FormatURLCode(utfLocalPath);
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		DownloadFileName = inParameter.LocalPath;
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			static jmethodID Method = Env->GetStaticMethodID(Class, "AndroidOSS_DownloadFile", "(Ljava/lang/String;Ljava/lang/String;I)Z");
			jstring netFile = Env->NewStringUTF(urlStr.c_str());
			jstring localFile = Env->NewStringUTF(localStr.c_str());
			UE_LOG(LogOSS, Log, TEXT("OSS : begin download file %s"), *ossPath);
			Env->CallStaticBooleanMethod(Class, Method, netFile, localFile, CurrentDownloadIndex);
			Env->DeleteLocalRef(netFile);
			Env->DeleteLocalRef(localFile);
		}
		Env->DeleteLocalRef(Class);
	}
	else {
		UE_LOG(LogOSS, Log, TEXT("OSS : failed to get jave environment"));
	}
#endif
}

void FAndroidOSSDownload::InnerUploadProcess(const FOssReqeustParameter &inParameter)
{

#if PLATFORM_ANDROID
	FString ossPath = FString::Printf(TEXT("GameResource/%s"), *inParameter.OssPath);
	const string utfLocalPath(TCHAR_TO_UTF8(*inParameter.LocalPath));
	const string utfOSSPath(TCHAR_TO_UTF8(*ossPath));

	string urlStr = FormatURLCode(utfOSSPath);
	string localStr = FormatURLCode(utfLocalPath);
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		UploadFileName = inParameter.LocalPath;
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			bIsSingleUploading = true;
			static jmethodID Method = Env->GetStaticMethodID(Class, "AndroidOSS_UploadFile", "(Ljava/lang/String;Ljava/lang/String;I)Z");
			jstring netFile = Env->NewStringUTF(urlStr.c_str());
			jstring localFile = Env->NewStringUTF(localStr.c_str());
			UE_LOG(LogOSS, Log, TEXT("OSS : begin Upload file %s"), *ossPath);
			Env->CallStaticBooleanMethod(Class, Method, netFile, localFile, CurrentUploadIndex);
			Env->DeleteLocalRef(netFile);
			Env->DeleteLocalRef(localFile);
		}
		Env->DeleteLocalRef(Class);
	}
	else {
		UE_LOG(LogOSS, Log, TEXT("OSS : failed to get jave environment"));
	}
#endif
}

bool FAndroidOSSDownload::Tick(float DeltaTime)
{
	IOSSDownload::Tick(DeltaTime);
	//进度.
	if (bIsSingleDownloading)
	{
		UE_LOG(LogOSS, Log, TEXT("PendingDownloadParameters:%i==%d==%d"), PendingDownloadParameters.Num(), DownloadProgressCurrent, DownloadProgressTotal);
		//UE_LOG(LogOSS, Log, TEXT("DOWNLOAD (%s) %d-%d"), *DownloadFileName, DownloadProgressCurrent, DownloadProgressTotal);
		OnFileDownloadProgress.Execute(FPaths::GetBaseFilename(DownloadFileName), DownloadProgressCurrent, DownloadProgressTotal == 0 ? INT32_MAX : DownloadProgressTotal);
		if (DownloadProgressCurrent >= DownloadProgressTotal && DownloadProgressTotal!=0)
		{
			UE_LOG(LogOSS, Log, TEXT("PendingDownloadPara~~~meters:%i==%d==%d"), PendingDownloadParameters.Num(), DownloadProgressCurrent, DownloadProgressTotal);
			// 表示下载完成一个.
			bIsSingleDownloading = false;

			DownloadProgressCurrent = 0;

			DownloadProgressTotal = 0;

			if (PendingDownloadParameters.Num() == 0)
			{
				bIsDownloadFinish = true;

				UE_LOG(LogOSS, Log, TEXT("PendingDownloadParameter！！！！s:%i==%d==%d"), PendingDownloadParameters.Num(), DownloadProgressCurrent, DownloadProgressTotal);
			}
		}
	}


	if (bIsSingleUploading)
	{
		UE_LOG(LogOSS, Log, TEXT("UPLOAD (%s) %d-%d"), *UploadFileName, UploadProgressCurrent, UploadProgressTotal);
		OnFileUploadProgress.Execute(FPaths::GetBaseFilename(UploadFileName), UploadProgressCurrent, UploadProgressTotal == 0 ? INT32_MAX : UploadProgressTotal);
		if (UploadProgressCurrent >= UploadProgressTotal && UploadProgressTotal!=0)
		{
			// 表示下载完成一个.
			bIsSingleUploading = false;

			UploadProgressCurrent = 1;

			UploadProgressTotal = 100;

			if (PendingUploadParameters.Num() == 0)
			{
				bIsUploadFinish = true;
			}
		}
	}

	//完成
	if (bIsDownloadFinish)
	{
		bIsDownloadFinish = false;
		UE_LOG(LogOSS, Log, TEXT("OnFileDownloadFinish.Execute()!!!"));

		OnFileDownloadFinish.Execute();
		//OnFileDownloadFinish.Unbind();
	}
	if (bIsUploadFinish)
	{
		bIsUploadFinish = false;
		OnFileUploadFinish.Execute();
	}

	// 处理下载
	if (PendingDownloadParameters.Num() > 0 && !bIsSingleDownloading)
	{
		DownloadFinishMutex.lock();
		bIsSingleDownloading = true;
		UE_LOG(LogOSS, Log, TEXT("TotalDownloadFiles:%i,PendingDownloadParameters:%i,CurrentDownloadIndex:%i"), TotalDownloadFiles.Num(), PendingDownloadParameters.Num(), CurrentDownloadIndex);
		InnerDownloadProcess(PendingDownloadParameters[PendingDownloadParameters.Num() - 1]);
		PendingDownloadParameters.RemoveAt(PendingDownloadParameters.Num() - 1);
		
		DownloadFinishMutex.unlock();
	}


	if (PendingUploadParameters.Num() > 0 && !bIsSingleUploading)
	{
		bIsSingleUploading = true;
		InnerUploadProcess(PendingUploadParameters[PendingUploadParameters.Num() - 1]);
		PendingUploadParameters.RemoveAt(PendingUploadParameters.Num() - 1);
	}

	return true;
}


void FAndroidOSSDownload::DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{
	UE_LOG(LogOSS, Log, TEXT("NEW PENDING DOWNLOAD REQUEST..."));
	PendingDownloadParameters = inParameters;

	OnFileDownloadProgress = inDelegate;
	OnFileDownloadFinish = inFinishDelegate;
	CurrentDownloadIndex = -1;
	//TotalDownloadFiles = PendingDownloadParameters;
	DownloadErrorParameters.Empty();
}

void FAndroidOSSDownload::UploadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileUploadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{
	UE_LOG(LogOSS, Log, TEXT("NEW PENDING UPLOAD REQUEST..."));
	PendingUploadParameters = inParameters;
	OnFileUploadProgress = inDelegate;
	OnFileUploadFinish = inFinishDelegate;
	CurrentUploadIndex = -1;
}

void FAndroidOSSDownload::UpdateSingleDownloadProgress(int32 inFlag, int32 inCurSize, int32 inTotalSize)
{
	if (!bIsSingleDownloading)
	{
		return;
	}
	DownloadProgressCurrent = inCurSize;
	DownloadProgressTotal = inTotalSize;

	UE_LOG(LogOSS, Log, TEXT("Finish download file111111111== %d,==%d==%d"), DownloadProgressCurrent, DownloadProgressTotal, bIsSingleDownloading);

	if (DownloadProgressCurrent == DownloadProgressTotal)
	{
		if (DownloadProgressTotal == 0)
			DownloadProgressTotal = 1;
		DownloadProgressCurrent = DownloadProgressCurrent - 1;
	}
	/*if (DownloadProgressTotal == 0)
		DownloadProgressTotal = 10000;

	if (DownloadProgressCurrent == DownloadProgressTotal)
		DownloadProgressCurrent = DownloadProgressCurrent - 1;*/
}

void FAndroidOSSDownload::FinishSingleDownload(int32 inFlag,bool success)
{
    if(success)
    {
        DownloadProgressTotal = DownloadProgressTotal==0?0: DownloadProgressTotal;
        DownloadProgressCurrent = DownloadProgressTotal;
    }else
    {
        bIsSingleDownloading = false;
        bIsDownloadFinish = false;
        PendingDownloadParameters.Empty();
        OnFileDownloadFinish.Execute(false);

		UE_LOG(LogOSS, Log, TEXT("Finish download file== %d,==%d==%d"), DownloadProgressCurrent, DownloadProgressTotal, PendingDownloadParameters.Num());
    }

	UE_LOG(LogOSS, Log, TEXT("Finish download file== %d,==%d==%d"), DownloadProgressCurrent, DownloadProgressTotal, bIsSingleDownloading);
}


void FAndroidOSSDownload::DownloadFailedHandle(int32 inFlag)
{
	/*if (inFlag != CurrentDownloadIndex)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("Download flag miss match  in:%d - cur%d"), inFlag, CurrentDownloadIndex)));
		return;
	}*/
	UE_LOG(LogOSS, Log, TEXT("Finish @@@@@ file== %s"), *DownloadFileName);
	DownloadProgressTotal = DownloadProgressTotal == 0 ? 1000 : DownloadProgressTotal;

	DownloadProgressCurrent = DownloadProgressTotal;

	//DownloadFailedFiles.Add(TotalDownloadFiles[inFlag]);
}

void FAndroidOSSDownload::UpdateSingleUploadProgress(int32 inFlag, int32 inCurSize, int32 inTotalSize)
{
	if (!bIsSingleUploading)
	{
		return;
	}
	UploadProgressCurrent = inCurSize;
	UploadProgressTotal = inTotalSize;

	if (inTotalSize == 0)
		UploadProgressTotal = 10000;

	if (UploadProgressCurrent == UploadProgressTotal)
		UploadProgressCurrent = UploadProgressCurrent - 1;
}

void FAndroidOSSDownload::FinishSingleUpload(int32 inFlag)
{
	UploadProgressTotal = UploadProgressTotal == 0 ? INT32_MAX : UploadProgressTotal;

	UploadProgressCurrent = UploadProgressTotal;
	UE_LOG(LogOSS, Log, TEXT("Finish Upload file %s"), *UploadFileName);
}

void FAndroidOSSDownload::UploadFailedHandle(int32 inFlag)
{

}




