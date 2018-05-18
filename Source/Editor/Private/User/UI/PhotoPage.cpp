// Fill out your copyright notice in the Description page of Project Settings.

#include "PhotoPage.h"
#include "UIManager.h"
#include "OSSModule.h"
#include "ImageMagick.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "GModuleManager.h"
#include "GPlatformFileManager.h"


UPhotoPage::UPhotoPage(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

bool UPhotoPage::OnStart()
{
	m_DesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	//	拍照按钮.
	if (UButton* mBtnPhoto = Cast<UButton>(GetWidgetFromName("Button_Photo")))
	{
		m_BtnPhoto = mBtnPhoto;

		//	拍照按钮--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnPhoto, this, &UPhotoPage::PhotoPicture);
	}

	//	从相册中选择按钮.
	if (UButton* mBtnPicture = Cast<UButton>(GetWidgetFromName("Button_Picture")))
	{
		m_BtnPicture = mBtnPicture;

		//	从相册中选择--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnPicture, this, &UPhotoPage::PhotoPicture);
	}

	//	取消按钮.
	if (UButton* mBtnCancel = Cast<UButton>(GetWidgetFromName("Button_Cancel")))
	{
		m_BtnCancel = mBtnCancel;

		//	取消--点击事件.
		UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnCancel, this, &UPhotoPage::PhotoPicture);
	}

	if (UTextBlock* mTextPhoto = Cast<UTextBlock>(GetWidgetFromName("Text_Photo")))
	{
		m_TextPhoto = mTextPhoto;

		//	获取文字.
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key128"));
		m_TextPhoto->SetText(FText::FromString(mText));
	}

	if (UTextBlock* mTextPicture = Cast<UTextBlock>(GetWidgetFromName("Text_Picture")))
	{
		m_TextPicture = mTextPicture;

		//	获取文字.
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key129"));
		m_TextPicture->SetText(FText::FromString(mText));
	}

	if (UTextBlock* mTextCancel = Cast<UTextBlock>(GetWidgetFromName("Text_Cancel")))
	{
		m_TextCancel = mTextCancel;

		//	获取文字.
		FString mText = GLanguageTool::GetInstance()->GetLanguage(TEXT("key130"));
		m_TextCancel->SetText(FText::FromString(mText));
	}
	return	true;
}

void UPhotoPage::OnInit()
{

}

void UPhotoPage::OnRelease()
{
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnPhoto);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnPicture);
	UUIManager::GetInstance()->RemoveButtonClick(m_BtnCancel);
}

void UPhotoPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UPhotoPage::OnTick(float InDeltaTimeSecond)
{

}

void UPhotoPage::PhotoPicture(int32 _pid)
{
	DesignModule *mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	if (_pid == 1)
	{
		//UE_LOG(LogTemp, Log, TEXT("PhotoPicture");

		this->SetVisibility(ESlateVisibility::Hidden);

		if(mDesignModule->m_UploadPattern == true)
			IImageMagickModule::Get().OpenCamera(FOnOpenReresult::CreateUObject(this, &UPhotoPage::CBOnOpenPattern));
		else
			IImageMagickModule::Get().OpenCamera(FOnOpenReresult::CreateUObject(this, &UPhotoPage::CBOnOpenPicture));

		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);
		
	}
	if (_pid == 2)
	{
		//UE_LOG(LogTemp, Log, TEXT("PhotoPicture");

		this->SetVisibility(ESlateVisibility::Hidden);
		//打开相册.
		if (mDesignModule->m_UploadPattern == true)
			IImageMagickModule::Get().OpenSelectPattern(FOnOpenReresult::CreateUObject(this, &UPhotoPage::CBOnOpenPattern));
		else
			IImageMagickModule::Get().OpenSelectPattern(FOnOpenReresult::CreateUObject(this, &UPhotoPage::CBOnOpenPicture));

		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);
		
	}
	if (_pid == 3)
	{
		//UE_LOG(LogTemp, Log, TEXT("remove picture");
		this->RemoveFromViewport();
	}
}

void UPhotoPage::GetUploadPattern()
{
	IImageMagickModule::Get().OpenSelectPattern(FOnOpenReresult::CreateUObject(this, &UPhotoPage::CBOnOpenPattern));
}

void UPhotoPage::CBOnOpenPattern(bool isSuccess, FString inIconFilePath, FString inFilePath, FVector2D inDPI, FVector2D inRes)
{
	m_DesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	FString FileName;
	FString IconFileName;

	FString ossIconUrl;
	FString nativeIconUrl;
	FString ossTextureUrl;
	FString nativeTextureUrl;

	float cm_width = inRes.X / inDPI.X*2.54;
	float cm_height = inRes.Y / inDPI.Y*2.54;

#if PLATFORM_ANDROID

	FileName = FGuid::NewGuid().ToString();
	IconFileName = FGuid::NewGuid().ToString();

	ossIconUrl = GPlatformFileManager::GetAndroidOssUrl(inIconFilePath, IconFileName);
	nativeIconUrl = GPlatformFileManager::GetAndroidNativeFullUrl(inIconFilePath, IconFileName);

	ossTextureUrl = GPlatformFileManager::GetAndroidOssUrl(inFilePath, FileName);
	nativeTextureUrl = GPlatformFileManager::GetAndroidNativeFullUrl(inFilePath, FileName);

	this->RemoveFromViewport();

	if (isSuccess)
	{
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeIconUrl);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeTextureUrl);

		IImageMagickModule::Get().CopyTo(inIconFilePath, nativeIconUrl);
		IImageMagickModule::Get().CopyTo(inFilePath, nativeTextureUrl);

		//删除文件.
		/*if (GPlatformFileManager::FileExists(*inIconFilePath))
		{
			GPlatformFileManager::DeleteFile(*inIconFilePath);
		}
		if (GPlatformFileManager::FileExists(*inFilePath))
		{
			GPlatformFileManager::DeleteFile(*inFilePath);
		}*/

		m_DesignModule->UploadPatternToOss(ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	}
	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();
	}
	UE_LOG(LogTemp, Log, TEXT("CBOnOpenPattern"));

#elif PLATFORM_IOS
	UE_LOG(LogTemp, Log, TEXT("开始复制图片"));

	FileName = FGuid::NewGuid().ToString();
	IconFileName = FGuid::NewGuid().ToString();

	// ios特定获取的地方.
	ossIconUrl = GPlatformFileManager::GetIOSOssUrl(inIconFilePath, IconFileName);
	nativeIconUrl = GPlatformFileManager::GetIOSNativeFullUrl(inIconFilePath, IconFileName);

	ossTextureUrl = GPlatformFileManager::GetIOSOssUrl(inFilePath, FileName);
	nativeTextureUrl = GPlatformFileManager::GetIOSNativeFullUrl(inFilePath, FileName);

	this->RemoveFromViewport();

	UE_LOG(LogTemp, Log, TEXT("结束复制图片：=%s"), *inIconFilePath);
	UE_LOG(LogTemp, Log, TEXT("结束复制图片：%d=%s"), isSuccess ? 1 : 0, *nativeIconUrl);
	UE_LOG(LogTemp, Log, TEXT(" 结束复制图片1：%s "), *ossTextureUrl);
	UE_LOG(LogTemp, Log, TEXT(" 结束复制图片2：%s "), *nativeTextureUrl);

	if (isSuccess)
	{
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeIconUrl);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeTextureUrl);

		IImageMagickModule::Get().CopyTo(inIconFilePath, nativeIconUrl);
		IImageMagickModule::Get().CopyTo(inFilePath, nativeTextureUrl);
		//删除文件.
		/*if (GPlatformFileManager::FileExists(*inIconFilePath))
		{
			GPlatformFileManager::DeleteFile(*inIconFilePath);
		}
		if (GPlatformFileManager::FileExists(*inFilePath))
		{
			GPlatformFileManager::DeleteFile(*inFilePath);
		}*/

		m_DesignModule->UploadPatternToOss(ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	}

	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();
	}
#endif

#if PLATFORM_WINDOWS
	nativeIconUrl = GPlatformFileManager::GetWindowsOssUrl(inIconFilePath);
	ossIconUrl = GPlatformFileManager::GetWindowsNativeFullUrl(inIconFilePath);

	nativeTextureUrl = GPlatformFileManager::GetWindowsOssUrl(inFilePath);
	ossTextureUrl = GPlatformFileManager::GetWindowsNativeFullUrl(inFilePath);

	this->RemoveFromViewport();

 	if (isSuccess)
	{
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

		m_DesignModule->UploadPatternToOss(ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	}
	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();
	}
#endif

	//OnOpenUploadPanel.Broadcast(isSuccess, ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("Open system pattern result(%s) dpi(%f,%f), resolution(%f,%f),width(%f),height(%f)"), isSuccess ? TEXT("TRUE") : TEXT("FALSE"), inDPI.X, inDPI.Y, inRes.X, inRes.Y, cm_width,cm_height));
	//UE_LOG(ClothEditor, Log, TEXT("Open system pattern result(%s) dpi(%f,%f), resolution(%f,%f)"), isSuccess ? TEXT("TRUE") : TEXT("FALSE"), inDPI.X, inDPI.Y, inRes.X, inRes.Y);
}

void UPhotoPage::GetUploadPicture()
{
	IImageMagickModule::Get().OpenSelectPattern(FOnOpenReresult::CreateUObject(this, &UPhotoPage::CBOnOpenPicture));
}

void UPhotoPage::CBOnOpenPicture(bool isSuccess, FString inIconFilePath, FString inFilePath, FVector2D inDPI, FVector2D inRes)
{
	m_DesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	FString FileName;
	FString IconFileName;

	FString ossIconUrl;
	FString nativeIconUrl;
	FString ossTextureUrl;
	FString nativeTextureUrl;

	float cm_width = inRes.X / inDPI.X*2.54;
	float cm_height = inRes.Y / inDPI.Y*2.54;

#if PLATFORM_ANDROID

	FileName = FGuid::NewGuid().ToString();
	IconFileName = FGuid::NewGuid().ToString();

	ossIconUrl = GPlatformFileManager::GetAndroidOssUrl(inIconFilePath, IconFileName);
	nativeIconUrl = GPlatformFileManager::GetAndroidNativeFullUrl(inIconFilePath, IconFileName);

	ossTextureUrl = GPlatformFileManager::GetAndroidOssUrl(inFilePath, FileName);
	nativeTextureUrl = GPlatformFileManager::GetAndroidNativeFullUrl(inFilePath, FileName);

	this->RemoveFromViewport();

	if (isSuccess)
	{
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeIconUrl);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeTextureUrl);

		IImageMagickModule::Get().CopyTo(inIconFilePath, nativeIconUrl);
		IImageMagickModule::Get().CopyTo(inFilePath, nativeTextureUrl);
		//删除文件.
		/*if (GPlatformFileManager::FileExists(*inIconFilePath))
		{
			GPlatformFileManager::DeleteFile(*inIconFilePath);
		}
		if (GPlatformFileManager::FileExists(*inFilePath))
		{
			GPlatformFileManager::DeleteFile(*inFilePath);
		}*/
		UE_LOG(LogTemp, Log, TEXT("zhoujia:UploadPictureToOss"));
		m_DesignModule->UploadPictureToOss(ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	}

	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();
	}
	UE_LOG(LogTemp, Log, TEXT("zhoujia:Open  pattern result(%s) ossIconUrl=%s, nativeIconUrl=%s"), isSuccess ? TEXT("TRUE") : TEXT("FALSE"), *ossIconUrl, *nativeIconUrl);
	UE_LOG(LogTemp, Log, TEXT("zhoujia:Open  pattern result(%s) ossTextureUrl=%s, nativeTextureUrl=%s"), isSuccess ? TEXT("TRUE") : TEXT("FALSE"), *ossTextureUrl, *nativeTextureUrl);
	UE_LOG(LogTemp, Log, TEXT("zhoujia:Open  pattern result(%s) inIconFilePath=%s, inFilePath=%s"), isSuccess ? TEXT("TRUE") : TEXT("FALSE"), *inIconFilePath,*inFilePath);


#elif PLATFORM_IOS
	UE_LOG(LogTemp, Log, TEXT("开始复制图片"));

	FileName = FGuid::NewGuid().ToString();
	IconFileName = FGuid::NewGuid().ToString();

	// ios特定获取的地方.
	ossIconUrl = GPlatformFileManager::GetIOSOssUrl(inIconFilePath, IconFileName);
	nativeIconUrl = GPlatformFileManager::GetIOSNativeFullUrl(inIconFilePath, IconFileName);

	ossTextureUrl = GPlatformFileManager::GetIOSOssUrl(inFilePath, FileName);
	nativeTextureUrl = GPlatformFileManager::GetIOSNativeFullUrl(inFilePath, FileName);

	UE_LOG(LogTemp, Log, TEXT("结束复制图片：=%s"), *inIconFilePath);
	UE_LOG(LogTemp, Log, TEXT("结束复制图片：%d=%s"), isSuccess ? 1 : 0, *nativeIconUrl);
	UE_LOG(LogTemp, Log, TEXT(" 结束复制图片1：%s "), *ossTextureUrl);
	UE_LOG(LogTemp, Log, TEXT(" 结束复制图片2：%s "), *nativeTextureUrl);

	this->RemoveFromViewport();
	if (isSuccess)
	{
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeIconUrl);

		GPlatformFileManager::RecursiveCreateDirectory(*nativeTextureUrl);

		IImageMagickModule::Get().CopyTo(inIconFilePath, nativeIconUrl);
		IImageMagickModule::Get().CopyTo(inFilePath, nativeTextureUrl);
		//删除文件.
		/*if (GPlatformFileManager::FileExists(*inIconFilePath))
		{
			GPlatformFileManager::DeleteFile(*inIconFilePath);
		}
		if (GPlatformFileManager::FileExists(*inFilePath))
		{
			GPlatformFileManager::DeleteFile(*inFilePath);
		}*/

		m_DesignModule->UploadPictureToOss(ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	}

	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();
	}

#endif

#if PLATFORM_WINDOWS
	nativeIconUrl = GPlatformFileManager::GetWindowsOssUrl(inIconFilePath);
	ossIconUrl = GPlatformFileManager::GetWindowsNativeFullUrl(inIconFilePath);

	nativeTextureUrl = GPlatformFileManager::GetWindowsOssUrl(inFilePath);
	ossTextureUrl = GPlatformFileManager::GetWindowsNativeFullUrl(inFilePath);

	this->RemoveFromViewport();
	if (isSuccess)
	{
		FString mTipContent = GLanguageTool::GetInstance()->GetLanguage(TEXT("key144"));

		UUIManager::GetInstance()->OpenMiddleTip(mTipContent, 0.0f);

		m_DesignModule->UploadPictureToOss(ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	}

	else
	{
		UUIManager::GetInstance()->RemoveAllAlertUI();
	}
#endif

	//OnOpenUploadPanel.Broadcast(isSuccess, ossIconUrl, nativeIconUrl, ossTextureUrl, nativeTextureUrl, cm_width, cm_height);
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString::Printf(TEXT("Open system pattern result(%s) dpi(%f,%f), resolution(%f,%f),width(%f),height(%f)"), isSuccess ? TEXT("TRUE") : TEXT("FALSE"), inDPI.X, inDPI.Y, inRes.X, inRes.Y, cm_width,cm_height));
	//UE_LOG(ClothEditor, Log, TEXT("Open system pattern result(%s) dpi(%f,%f), resolution(%f,%f)"), isSuccess ? TEXT("TRUE") : TEXT("FALSE"), inDPI.X, inDPI.Y, inRes.X, inRes.Y);

}
