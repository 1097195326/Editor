// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "Modules/ModuleManager.h"
#include "HttpModule.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IHttpResponse.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "GTimerMgr.h"
#include "GFileHelper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "RunTime/UMG/Public/Blueprint/AsyncTaskDownloadImage.h"
#include "DownloadImageTool.h"


UDownloadImageTool * UDownloadImageTool::m_instance = nullptr;

UDownloadImageTool::UDownloadImageTool(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		
	}
	AddToRoot();
}

UDownloadImageTool * UDownloadImageTool::GetInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = NewObject<UDownloadImageTool>();

		m_instance->GLoad();
	}
	return m_instance;
}

UTexture2D * UDownloadImageTool::GetTexture2dByUnit8Array(const TArray<uint8> & _array, int32 _lenth)
{
	UTexture2D* Texture = nullptr;
	
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrappers[3] =
	{
		ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
	};

	for (auto ImageWrapper : ImageWrappers)
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(_array.GetData(), _lenth))
		{
			const TArray<uint8>* UncompressedRGBA = nullptr;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedRGBA))
			{
				Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
				if (Texture != nullptr)
				{
					void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

					FMemory::Memcpy(TextureData, UncompressedRGBA->GetData(), UncompressedRGBA->Num());

					Texture->PlatformData->Mips[0].BulkData.Unlock();

					Texture->UpdateResource();
				}
			}
		}
	}
	return Texture;
}

void UDownloadImageTool::StartDownload()
{
	FHttpModule::Get().SetHttpTimeout(60);

	TSharedRef<IHttpRequest> mHttpRequest = FHttpModule::Get().CreateRequest();

	mHttpRequest->SetURL(m_url);

	mHttpRequest->SetVerb(TEXT("GET"));

	mHttpRequest->OnProcessRequestComplete() = FHttpRequestCompleteDelegate::CreateUObject(this, &UDownloadImageTool::DownloadFinish);

	mHttpRequest->ProcessRequest();

	m_HttpRequest.Add(mHttpRequest);
}

void UDownloadImageTool::StartDownloadImage(FString _imageUrl)
{
	FHttpModule::Get().SetHttpTimeout(60);
    
    m_downloadUrl = _imageUrl;

	TSharedRef<IHttpRequest> mHttpRequest = FHttpModule::Get().CreateRequest();

	mHttpRequest->SetURL(_imageUrl);

	mHttpRequest->SetVerb(TEXT("GET"));

	mHttpRequest->OnProcessRequestComplete() = FHttpRequestCompleteDelegate::CreateUObject(this, &UDownloadImageTool::DownloadImageFinish);

	mHttpRequest->ProcessRequest();

	m_HttpRequest.Add(mHttpRequest);
}

void UDownloadImageTool::CancelDownload()
{
	m_destroy = true;

	m_callbackFunc = nullptr;

	if (!m_downloadFinish && m_HttpRequest.Num()>0)
	{
		m_HttpRequest[0]->CancelRequest();
	}
	RemoveFromRoot();
}

void UDownloadImageTool::GLoad()
{
	// 启动update.
	GTimerMgr::GetInstance()->RegisterGameUpdateHandler(this);
}

void UDownloadImageTool::GRelease()
{
	GTimerMgr::GetInstance()->RemoveGameUpdateHandler(this);
	if (nullptr != m_instance)
	{
		m_instance->RemoveFromRoot();
	}
	m_instance = nullptr;
}

void UDownloadImageTool::OnUpdate(float DeltaTime)
{
	// 先拿到队列里的第一个值.
	if (m_downloadQuene.Num()>0)
	{
		DownloadImageStruct * mStruct = m_downloadQuene[0];

		if (mStruct->m_params.Num() == 0)
		{
			m_downloadQuene.RemoveAt(0);

			mStruct->m_callbackFunc();

			delete mStruct;
		}
		else
		{
			// 拿到队列里第一个下载的东西.
			m_currentDownloadParam = mStruct->m_params[0];

			if (m_currentDownloadParam->m_isDownloadFinish == EnumDownloadImage::WAITTING_DOWNLOAD)
			{
				m_currentDownloadParam->m_isDownloadFinish = EnumDownloadImage::DOWNLOADING;

				StartDownloadImage(m_currentDownloadParam->m_downloadUrl); 
			}
			else if(m_currentDownloadParam->m_isDownloadFinish == EnumDownloadImage::DOWNLOAD_FINISH)
			{
				mStruct->m_params.RemoveAt(0);

				delete m_currentDownloadParam;
			}
		}
	}
}

void UDownloadImageTool::DownloadImage(DownloadImageStruct * _downloadStruct)
{
	m_downloadQuene.Add(_downloadStruct);   
}

void UDownloadImageTool::DownloadFinish(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)
{
	if (m_destroy)
	{
		return;
	}

	m_downloadFinish = true;
	
	UTexture2D * mTexture2d = nullptr;

	if (inSuccess)
	{
		if (inResp.IsValid())
		{
			const TArray<uint8> & mContent = inResp->GetContent();

			int32 mContentLenght = mContent.Num();

			mTexture2d = GetTexture2dByUnit8Array(mContent, mContentLenght);

			m_repeatCount = 0;

			if (nullptr != m_callbackFunc)
				m_callbackFunc(inSuccess, m_penetrateId, m_url, mTexture2d, this);
		}
		else
		{
			if (m_repeatCount < m_maxRepeatCount)
			{
				m_repeatCount = m_repeatCount + 1;
				StartDownload();
			}
			else
			{
				m_repeatCount = 0;
				if (nullptr != m_callbackFunc)
					m_callbackFunc(inSuccess, m_penetrateId, m_url, mTexture2d, this);
			}
		}
	}
	else
	{
		if (m_repeatCount < m_maxRepeatCount)
		{
			m_repeatCount = m_repeatCount + 1;
			StartDownload();
		}
		else
		{
			m_repeatCount = 0;
			if (nullptr != m_callbackFunc)
				m_callbackFunc(inSuccess, m_penetrateId, m_url, mTexture2d, this);
		}
	}
}

void UDownloadImageTool::DownloadImageFinish(FHttpRequestPtr inReq, FHttpResponsePtr inResp, bool inSuccess)
{
	// 图片下载完成以后保存到本地.
	if (inSuccess)
	{
		if (inResp.IsValid())
		{
			m_currentDownloadParam->m_isDownloadFinish = EnumDownloadImage::DOWNLOAD_FINISH;

			const TArray<uint8> & mContent = inResp->GetContent();

			int32 mContentLenght = inResp->GetContentLength();

			GFileHelper::SaveArrayToFile(mContent, *m_currentDownloadParam->m_savePath);

			m_repeatCount = 0;
		}
		else
		{
			if (m_repeatCount<m_maxRepeatCount)
			{
				m_repeatCount = m_repeatCount + 1;
				m_currentDownloadParam->m_isDownloadFinish = EnumDownloadImage::WAITTING_DOWNLOAD;
			}
			else
			{
				m_repeatCount = 0;
				m_currentDownloadParam->m_isDownloadFinish = EnumDownloadImage::DOWNLOAD_FINISH;
			}
		}
	}
	else
	{
		if (m_repeatCount < m_maxRepeatCount)
		{
			m_repeatCount = m_repeatCount + 1;
			m_currentDownloadParam->m_isDownloadFinish = EnumDownloadImage::WAITTING_DOWNLOAD;
		}
		else
		{
			m_repeatCount = 0;
			m_currentDownloadParam->m_isDownloadFinish = EnumDownloadImage::DOWNLOAD_FINISH;
		}
	}
}
