#include "OSSModule.h"
#if PLATFORM_WINDOWS || PLATFORM_MAC
#include "Misc/Paths.h"
#include "FOSSDownload.h"
#include <aos_util.h>
#include <aos_string.h>
#include <aos_status.h>
#include <oss_auth.h>
#include <oss_util.h>
#include <oss_api.h>

using namespace std;

FOSSDownload* FOSSDownload::Runnable = nullptr;


string CurrentUploadFile;
void UploadProgress(int64_t consumed_bytes, int64_t total_bytes)
{
	FOSSDownload::Get()->m_uploadProgressCurrent = consumed_bytes;
	FOSSDownload::Get()->m_uploadProgressTotal = consumed_bytes;
	UE_LOG(LogOSS, Log, TEXT("upload %ld,%ld"), consumed_bytes, total_bytes);
}

string CurrentDownloadFile;
void DownloadProgress(int64_t consumed_bytes, int64_t total_bytes)
{
	FOSSDownload::Get()->m_downloadProgressCurrent = consumed_bytes;
	FOSSDownload::Get()->m_downloadProgressTotal = total_bytes;
	UE_LOG(LogOSS, Log, TEXT("download %ld,%ld"), FOSSDownload::Get()->m_downloadProgressCurrent, FOSSDownload::Get()->m_downloadProgressTotal);
}

FOSSDownload::FOSSDownload() :
	m_downloadProgressCurrent(0),
	m_downloadProgressTotal(0),
	m_uploadProgressCurrent(0),
	m_uploadProgressTotal(0)
{
}
FOSSDownload::~FOSSDownload()
{
	//if(!m_canStop)
		//Stop();
}

FOSSDownload * FOSSDownload::Get()
{
	if (!Runnable)
	{
		Runnable = new FOSSDownload();
	}
	return Runnable;
}


bool FOSSDownload::Init(FString _AosEndPoint, FString _AosAccessKeyID, FString _AosAccessKeySecret, FString _AosBucket)
{
	IOSSDownload::Init(_AosEndPoint, _AosAccessKeyID, _AosAccessKeySecret, _AosBucket);
	m_isInit = false;
	// 初始化.
	if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
		return false;
	}

	aos_pool_create(&AosPool, NULL);
	AosOptions = oss_request_options_create(AosPool);

	AosOptions->config = oss_config_create(AosOptions->pool);


	mAosEndPoint = TCHAR_TO_UTF8(*AosEndPoint);
	mAosAccessKeyID = TCHAR_TO_UTF8(*AosAccessKeyID);
	mAosAccessKeySecret = TCHAR_TO_UTF8(*AosAccessKeySecret);
	

	aos_str_set(&AosOptions->config->endpoint, mAosEndPoint.c_str());
	aos_str_set(&AosOptions->config->access_key_id, mAosAccessKeyID.c_str());
	aos_str_set(&AosOptions->config->access_key_secret, mAosAccessKeySecret.c_str());
	AosOptions->config->is_cname = 0;
	AosOptions->ctl = aos_http_controller_create(AosOptions->pool, 0);

	TickDelegate = FTickerDelegate::CreateRaw(this, &FOSSDownload::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);

	// 启动一个线程.
	m_thread = new std::thread(std::bind(&FOSSDownload::DonwnloadOrUploadInThread, this));
	m_isInit = true;
	return true;
}

uint32 FOSSDownload::DonwnloadOrUploadInThread()
{
	while (true)
	{
		if (!m_isThreadBreak)
		{
			if (PendingDownloadParameters.Num() > 0)
			{

				//UE_LOG(LogOSS, Log, TEXT("down file number (1,%d)"), PendingDownloadParameters.Num());
				InnerDownloadProcess(PendingDownloadParameters);
				PendingDownloadParameters.Empty();

			}

			if (PendingUploadParameters.Num() > 0)
			{

				//UE_LOG(LogOSS, Log, TEXT("upload number (1,%d)"), PendingUploadParameters.Num());
				InnerUploadProcess(PendingUploadParameters);
				PendingUploadParameters.Empty();

			}
		}
		else
		{
			//;
			delete this;
			break;
		}

		// 60帧一次发送或者接收消息.
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;

}

void FOSSDownload::Stop()
{
	// 退出线程.
	m_isThreadBreak = true;

	if (m_isInit)
	{
		m_isInit = false;
		/* 释放内存pool资源，包括了通过pool分配的内存，比如options等*/
		aos_pool_destroy(AosPool);
		/* 释放全局资源，应该放在程序结束前 */
		aos_http_io_deinitialize();
		FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	}
}

bool FOSSDownload::CanDownload() const
{
	return PendingDownloadParameters.Num() == 0;
}

bool FOSSDownload::CanUpload() const
{
	return PendingUploadParameters.Num() == 0;
}

void FOSSDownload::GRelease()
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	Runnable = nullptr;
	m_isThreadBreak = true;
}

void FOSSDownload::ExitGame()
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	Runnable = nullptr;
	m_isThreadBreak = true;
	m_canStop = true;
	if (m_isInit && m_thread && m_thread->joinable())
	{
		m_thread->join();
	}
	
}

void FOSSDownload::InnerDownloadProcess(const TArray<FOssReqeustParameter> &inParameters)
{
	m_isDownloadFinish = false;

	m_downloadProgressCurrent = 0.0;

	m_isFail = false;

	for (const FOssReqeustParameter &elem : inParameters)
	{
		if (m_isThreadBreak)
			return;
		FString mOSSPath = FString::Printf(TEXT("GameResource/%s"), *elem.OssPath);
		const string inSaveFilePath(TCHAR_TO_UTF8(*elem.LocalPath));
		const string inDownloadReletivePath(TCHAR_TO_UTF8(*mOSSPath));


		UE_LOG(LogOSS, Log, TEXT("Download ====: %s===%s"), *mOSSPath,*elem.LocalPath);
		if (!DirIsExist(inSaveFilePath) && !CreateDir(inSaveFilePath))
		{
			UE_LOG(LogOSS, Error, TEXT("Cannot create dir %s"), *elem.LocalPath);
			return;
		}
		if (DirIsExist(inSaveFilePath))
		{
			UE_LOG(LogOSS,Log,TEXT("inSaveFilePath is ture"));
		}
		else
		{
			UE_LOG(LogOSS, Log, TEXT("inSaveFilePath is false"));
		}

		m_isDownloadFinish = false;
		string urlStr = FormatURLCode(inDownloadReletivePath);
		string localStr = FormatURLCode(inSaveFilePath);


		aos_string_t bucket;
		aos_string_t object;
		aos_string_t filePath;
		aos_str_set(&bucket, TCHAR_TO_UTF8(*AosBucket));
		aos_str_set(&object, urlStr.c_str());
		aos_str_set(&filePath, localStr.c_str());

		aos_status_t *s = oss_do_get_object_to_file(AosOptions, &bucket, &object, NULL, NULL, &filePath, DownloadProgress, NULL);
        UE_LOG(LogOSS, Log, TEXT("begin to Upload file %s to %s"), *elem.LocalPath, *elem.OssPath);
		CurrentDownloadFile = inDownloadReletivePath;

		if (aos_status_is_ok(s)) {
			UE_LOG(LogOSS, Log, TEXT("Success to download file %s to %s"), *mOSSPath, *elem.LocalPath);
			m_msg_received_mutex.lock();
			m_received_downloaded_queue.push_back(inDownloadReletivePath);

			m_msg_received_mutex.unlock();

			m_downloadProgressCurrent = m_downloadProgressTotal;
		}
		else {
			UE_LOG(LogOSS, Error, TEXT("failed to download file %s to %s code (%d),msg(%s)"), *mOSSPath, *elem.LocalPath, s->code, ANSI_TO_TCHAR(s->error_msg));
			m_downloadProgressCurrent = m_downloadProgressTotal = 0;
			m_msg_received_mutex.lock();
			m_received_downloaded_queue.push_back(inDownloadReletivePath);
			m_isFail = true;
			m_msg_received_mutex.unlock();
		}

		// 60帧一次发送或者接收消息.
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	if (m_isFail)
	{
		m_isDownloadFinish = false;
		m_received_downloaded_queue.clear();
		OnFileDownloadFinish.Execute(false);
		
	}
	else
	{
		m_isDownloadFinish = true;
	}
}

void FOSSDownload::InnerUploadProcess(const TArray<FOssReqeustParameter> &inParameters)
{
	m_isUploadFinish = false;

	m_uploadProgressCurrent = 0;
	m_uploadProgressTotal = 0;
	for (const FOssReqeustParameter &elem : inParameters)
	{
		if (m_isThreadBreak)
			return;
		FString ossPath = FString::Printf(TEXT("GameResource/%s"), *elem.OssPath);
		const string inSaveFilePath(TCHAR_TO_UTF8(*elem.LocalPath));
		const string inUploadFilePath(TCHAR_TO_UTF8(*ossPath));

		// 开始上传.
		if (!FileIsExist(inSaveFilePath))
		{
			return;
		}
		m_isUploadFinish = false;
		string urlStr = FormatURLCode(inUploadFilePath);
		string localStr = FormatURLCode(inSaveFilePath);
		aos_string_t bucket;
		aos_string_t object;
		aos_table_t *headers = NULL;
		aos_table_t *resp_headers = NULL;
		aos_status_t *s = NULL;
		aos_string_t file;

		aos_str_set(&bucket, TCHAR_TO_UTF8(*AosBucket));
		aos_str_set(&object, urlStr.c_str());
		aos_str_set(&file, localStr.c_str());

		CurrentUploadFile = inUploadFilePath;
		UE_LOG(LogOSS, Log, TEXT("begin to Upload file %s to %s"), *elem.LocalPath, *elem.OssPath);
		s = oss_do_put_object_from_file(AosOptions, &bucket, &object, &file, NULL, NULL, UploadProgress, NULL, NULL);
		if (aos_status_is_ok(s)) {
			UE_LOG(LogOSS, Log, TEXT("Success to Upload file %s to %s"), *elem.LocalPath, *elem.OssPath);
			m_msg_received_mutex.lock();
			m_received_uploaded_queue.push_back(inUploadFilePath);
			m_msg_received_mutex.unlock();

			m_uploadProgressCurrent = m_uploadProgressTotal;

		}
		else {
			m_uploadProgressCurrent = m_uploadProgressTotal;
			m_msg_received_mutex.lock();
			m_received_uploaded_queue.push_back(inUploadFilePath);
			m_msg_received_mutex.unlock();
			//printf("code(%d) , %s\n", s->code, s->error_msg);
			UE_LOG(LogOSS, Error, TEXT("failed to Upload file %s to %s code (%d),msg(%s)"), *elem.LocalPath, *elem.OssPath, s->code, ANSI_TO_TCHAR(s->error_msg));
		}
		// 60帧一次发送或者接收消息.
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	m_isUploadFinish = true;
}

bool FOSSDownload::Tick(float DeltaTime)
{
	IOSSDownload::Tick(DeltaTime);
	//UE_LOG(LogOSS, Log, TEXT("download============== %ld,%ld"), FOSSDownload::Get()->m_downloadProgressCurrent, FOSSDownload::Get()->m_downloadProgressTotal);

	if (!m_received_downloaded_queue.empty() && m_received_downloaded_queue.size() > 0)
	{
		// 遍历vector 发送 .
		m_msg_received_mutex.lock();
		auto iter = m_received_downloaded_queue.begin();

		while (iter != m_received_downloaded_queue.end())
		{
			string _path = *iter;

			OnFileDownloadProgress.Execute(FPaths::GetBaseFilename(_path.c_str()), 100, 100);

			++iter;
		}

		//  只需要释放数据不需要连内存都释放.
		m_received_downloaded_queue.clear();
		m_msg_received_mutex.unlock();
	}

	if (!m_received_uploaded_queue.empty() && m_received_uploaded_queue.size() > 0)
	{
		m_msg_received_mutex.lock();
		auto iter = m_received_uploaded_queue.begin();

		while (iter != m_received_uploaded_queue.end())
		{
			string _path = *iter;
			OnFileUploadProgress.Execute(FPaths::GetBaseFilename(_path.c_str()), m_uploadProgressCurrent, m_uploadProgressTotal == 0 ? INT32_MAX : m_uploadProgressTotal);
			++iter;
		}

		//  只需要释放数据不需要连内存都释放.
		m_received_uploaded_queue.clear();
		m_msg_received_mutex.unlock();
	}

	if (FOSSDownload::Get()->m_downloadProgressCurrent < FOSSDownload::Get()->m_downloadProgressTotal)
	{
		OnFileDownloadProgress.Execute(FPaths::GetBaseFilename(CurrentDownloadFile.c_str()), 
			FOSSDownload::Get()->m_downloadProgressCurrent,
			FOSSDownload::Get()->m_downloadProgressTotal == 0 ? INT32_MAX : FOSSDownload::Get()->m_downloadProgressTotal);
	}

	if (m_uploadProgressCurrent < m_uploadProgressTotal)
	{
		OnFileUploadProgress.Execute(FPaths::GetBaseFilename(CurrentUploadFile.c_str()), m_uploadProgressCurrent,  m_uploadProgressTotal == 0 ? INT32_MAX : m_uploadProgressTotal);
	}

	if (m_isDownloadFinish)
	{
		m_isDownloadFinish = false;
		/*if (m_isFail)
		{
			OnFileDownloadFinish.Execute(false);
		}
		else*/
		{
			OnFileDownloadFinish.Execute();
		}
		//OnFileDownloadFinish.Unbind();
		UE_LOG(LogOSS, Log, TEXT("download request finished!"));
	}
	if (m_isUploadFinish)
	{
		m_isUploadFinish = false;
		OnFileUploadFinish.Execute();
		//OnFileUploadFinish.Unbind();
		UE_LOG(LogOSS, Log, TEXT("Upload request finished!"));
	}


	return true;
}


void FOSSDownload::DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{

	PendingDownloadParameters = inParameters;
	OnFileDownloadProgress = inDelegate;
	OnFileDownloadFinish = inFinishDelegate;


}

void FOSSDownload::UploadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileUploadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate)
{
	PendingUploadParameters = inParameters;
	OnFileUploadProgress = inDelegate;
	OnFileUploadFinish = inFinishDelegate;

}
#endif








