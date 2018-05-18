// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorDirector.h"
#include "GHttpManager.h"
#include "LoginModule.h"
#include "ServerPrototype.h"
#include "PrototypeManager.h"
#include "UIManager.h"
#include "GController.h"
#include "PlatformUIPrototype.h"
#include "UIPrototype.h"
#include "GModuleManager.h"
#include "EncryptDecryptTool.h"
#include "ControllerPrototype.h"
#include "OSSHelper.h"
#include "HaoshaSDKBPLibrary.h"
#include "PakMountHelper.h"
#include "HotLoadManager.h"
#include "LoadDataModule.h"
#include "HaoshaSDKBPLibrary.h"
#include "HomeInfoManager.h"

void UEditorDirector::LoadXmlFile()
{
	UE_LOG(LogTemp, Log, TEXT("zhx : load XML file"));
	UHotLoadManager::GetInstance()->GetVersionXML();
}
void UEditorDirector::PakMount()
{
	UE_LOG(LogTemp, Log, TEXT("zhx : Pak Mount fuc"));
	UHotLoadManager::GetInstance()->PakMount();
}
bool UEditorDirector::MountLocalPakFiles(const FString &inFolder)
{
	TArray<FString> files;
	PakMountHelper::GetFiles(inFolder, files, false, TEXT("pak"));
	if (files.Num() == 0)
	{
		return true;
	}

	FString tmpFileCleanName;
	for (auto &elem : files)
	{
		tmpFileCleanName = FPaths::GetBaseFilename(elem);
		tmpFileCleanName.ReplaceInline(TEXT("___"), TEXT("/"));
		PakMountHelper::MountPak(elem, tmpFileCleanName);
	}

	return true;
}


void UEditorDirector::PostInitProperties()
{
    Super::PostInitProperties();
    
    //FCoreDelegates::ApplicationRegisteredForRemoteNotificationsDelegate.AddUObject(this, &UPlatformGameInstance::ApplicationRegisteredForRemoteNotificationsDelegate_Handler);
}

void UEditorDirector::ApplicationRegisteredForRemoteNotificationsDelegate_Handler(TArray<uint8> inArray)
{
    //UHaoshaSDKBPLibrary::ApplicationRegisteredForRemoteNotificationsDelegate_Handler(inArray);
}

void UEditorDirector::BeginDestroy()
{
    Super::BeginDestroy();
    FCoreDelegates::ApplicationRegisteredForRemoteNotificationsDelegate.RemoveAll(this);
}

void UEditorDirector::OnLoad()
{
    //UHaoshaSDKBPLibrary::HaosaInstallUncaughtExceptionHandler();
	
	// 初始化消息系统.
	MsgCenter::Instance()->GLoad();

	GModuleManager::GetInstance()->GLoad();

	// 加载所有的Modules .
	AARefClasses::GetInstance()->LoadAllModule();

	UHotLoadManager::GetInstance()->Init();

	HomeInfoManager::GetInstance()->GLoad();

	PrototypeManager::GetInstance()->GLoad();

	PrototypeManager::GetInstance()->LoadAllSystemPrototype();

	// 设置分辨率.
	UGameUserSettings* mGameUserSettings = GEngine->GetGameUserSettings();

	FIntPoint mPoint = mGameUserSettings->GetScreenResolution();

	float mIntProportion = (float)mPoint.X / (float)mPoint.Y;
	  
	FString mProportion = FString::Printf(TEXT("%0.2f"), mIntProportion);

	GController::m_proportion = mProportion;

	GController::m_screenHeight = mPoint.Y;

	GController::m_screenWidth = mPoint.X;

	UIPrototype * mUIPrototype = (UIPrototype *)(PrototypeManager::GetInstance()->GetPrototypeByPID(PrototypeConst::P_UIPROTOTYPE_ID));

	int32 mUIId = mUIPrototype->GetUIPrototypeId(GController::m_proportion);

	PlatformUIPrototype * mPlatformUIPrototype = (PlatformUIPrototype *)(PrototypeManager::GetInstance()->GetPrototypeByPID(mUIId));

	UUIManager::GetInstance()->LoadPrototype(mPlatformUIPrototype);

	// 初始化http.
	InitHttpManager();

	// 初始化OSS.
	InitOSS();

	// 初始化分享.
	UHaoshaSDKBPLibrary::InitLibrary(FString(TEXT("2314290489057")),
		FString(TEXT("7ce193684f8d7358adff74aede586e79")));

	if (GController::m_isDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 999999, FColor::Red, FString::Printf(TEXT("设备分辨率x=%d,y=%d"), mPoint.X,mPoint.Y));
	}

	//InitExption();
    
    //MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4026, this, &UEditorDirector::OnErrorResponse);
    
    // 给服务器发送异常信息.
	//new std::thread(std::bind(&UEditorDirector::SendErrorMsgThread, this));
}

void UEditorDirector::SendErrorMsgThread()
{
	SendErrorMsgToServer();
}

void UEditorDirector::OnRelease()
{
    MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 4026, this, &UEditorDirector::OnErrorResponse);
    
	MsgCenter::Instance()->GRelease();

	GModuleManager::GetInstance()->GRelease();

	UUIManager::GetInstance()->GRelease();

	PrototypeManager::GetInstance()->GRelease();
	
	GHttpManager::GetInstance()->GRelease();

	OSSHelper::GetInstance()->ExitGame();

	UHotLoadManager::GetInstance()->Destroy();
}

void UEditorDirector::InitHttpManager()
{
	int32 mServerPrototypeId = PrototypeConst::P_HTTP_WINDOWS_SERVER_ID;

	if (GController::m_platform == ENUM_PLATFORM::GANDROID)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_ANDROID_SERVER_ID;
	}
	else if (GController::m_platform == ENUM_PLATFORM::GIOS)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_IOS_SERVER_ID;
	}

	ServerPrototype * mServerPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ServerPrototype>(mServerPrototypeId);

	FString mSource = mServerPrototype->GetValueByKey(TEXT("source"));

	FString mShopCode = mServerPrototype->GetValueByKey(TEXT("shopCode"));

	FString mOS = mServerPrototype->GetValueByKey(TEXT("os"));

	FString mHttpUrl = mServerPrototype->GetValueByKey(TEXT("serverUrl"));

	EConvertibleLaptopMode Ev = FPlatformMisc::GetConvertibleLaptopMode();

	FString _model = TEXT("unknow");

	switch (Ev)
	{
	case EConvertibleLaptopMode::Laptop:_model = TEXT("Laptop"); break;

	case EConvertibleLaptopMode::Tablet:_model = TEXT("Tablet"); break;

	case EConvertibleLaptopMode::NotSupported:break;

	default:break;
	}

	// 设备版本.
	FString mVersion = FPlatformMisc::GetOSVersion();

#if PLATFORM_WINDOWS
	// 设备编号.
	FString mDeviceCode = FPlatformMisc::GetDeviceId();

	// 设备版本.
	FPlatformMisc::GetOSVersions(mOS, mVersion);
#else
	// 设备编号.
	FString mDeviceCode = UHaoshaSDKBPLibrary::GetDeviceType();

	// 设备版本.
	mVersion = UHaoshaSDKBPLibrary::GetDeviceVersion();
#endif

	FString mAppVersion = GController::m_appVersion;

	FString mApiVersion = mServerPrototype->GetValueByKey(TEXT("apiVersion"));

	FString mAppType = mServerPrototype->GetValueByKey(TEXT("appType"));

	FString mChannel = mServerPrototype->GetValueByKey(TEXT("channel"));

	GHttpManager::GetInstance()->CreateHaoshaHttp(mHttpUrl,mSource, mShopCode, mOS, mVersion, _model, mDeviceCode, mAppVersion, mApiVersion, mAppType, mChannel);

	// 初始化下载二进制的方法.
	GHttpManager::GetInstance()->CreateDownloadHttp();
}

void UEditorDirector::InitExption()
{
	int32 mServerPrototypeId = PrototypeConst::P_HTTP_WINDOWS_SERVER_ID;

	if (GController::m_platform == ENUM_PLATFORM::GANDROID)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_ANDROID_SERVER_ID;
	}
	else if (GController::m_platform == ENUM_PLATFORM::GIOS)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_IOS_SERVER_ID;
	}

	ServerPrototype * mServerPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ServerPrototype>(mServerPrototypeId);
	FString mApiVersion = mServerPrototype->GetValueByKey(TEXT("apiVersion"));
	FString mAppVersion = GController::m_appVersion;
	LoginModule * mModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");
	UHaoshaSDKBPLibrary::InitExceptionHandler(mAppVersion, mModule->GetUserData()->GetUserId(), mApiVersion);
}

void UEditorDirector::SendErrorMsgToServer()
{
    //FString mErrorMsg = TEXT("[{\"cpuType\":\"CPU_TYPE_ARM64\",\"apiVersion\":\"1.4.3\",\"version\":\"10.0.2\",\"occurTime\":\"2018-03-19 04:27:15\",\"exceptionInfo\":\"Signal 13 was raised.,(null)\",\"appType\":\"FAFA_SHOP_APP\",\"source\":\"FAFA_SHOP_APP_IOS\",\"os\":\"IOS\",\"storageFree\":\"33.5 GB\",\"level\":\"Error\",\"memory\":\"977.0 MB\",\"memoryFree\":\"653.4 MB\",\"storage\":\"54.9 GB\",\"account\":\"100886\",\"model\":\"iPhone 6 Plus\",\"appVersion\":\"1.4.5\",\"deviceCode\":\"1F878207-D594-4209-AD34-7C7E0AC08517\"}]");//UHaoshaSDKBPLibrary::GetErrorLog();
    FString mErrorMsg = UHaoshaSDKBPLibrary::GetErrorLog();

	UE_LOG(LogTemp, Log, TEXT("zhx : %s==error"), *mErrorMsg);
    
    if(mErrorMsg.IsEmpty() || mErrorMsg.Equals(TEXT("")))
       return;
    
    int32 mServerPrototypeId = PrototypeConst::P_HTTP_WINDOWS_SERVER_ID;
    
    if (GController::m_platform == ENUM_PLATFORM::GANDROID)
    {
        mServerPrototypeId = PrototypeConst::P_HTTP_ANDROID_SERVER_ID;
    }
    else if (GController::m_platform == ENUM_PLATFORM::GIOS)
    {
        mServerPrototypeId = PrototypeConst::P_HTTP_IOS_SERVER_ID;
    }
    
    ServerPrototype * mServerPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ServerPrototype>(mServerPrototypeId);

    UE_LOG(LogTemp,Log,TEXT("zhx : UEditorDirector::SendErrorMsgToServer%s"),*(mServerPrototype->GetValueByKey(TEXT("errorHttpUrl"))));
    
    FString mAppendUrl = mServerPrototype->GetValueByKey(TEXT("errorHttpUrl"));
    
    msg_ptr mHttp(new MsgHttp(MessageTypeHttpRequest,4026, mErrorMsg, mAppendUrl));
    
    MsgCenter::Instance()->SendMsg(mHttp);
}

void UEditorDirector::OnErrorResponse(msg_ptr _msg)
{
    UE_LOG(LogTemp,Log,TEXT("zhx : UEditorDirector::OnErrorResponse"));
    MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();
    
    if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
    {
        // 删除日志.
        UHaoshaSDKBPLibrary::DeleteExceptionInfo();
    }
    else
    {
        // 消息异常,自行根据逻辑判断是否重发.
        
    }
}

void UEditorDirector::InitOSS()
{
	ServerPrototype * mOssPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ServerPrototype>(PrototypeConst::P_OSS_SERVER_ID);

	FString mAosEndPoint = mOssPrototype->GetValueByKey(TEXT("AosEndPoint"));

	FString mAosAccessKeyID = mOssPrototype->GetValueByKey(TEXT("AosAccessKeyID"));

	FString mAosAccessKeySecret = mOssPrototype->GetValueByKey(TEXT("AosAccessKeySecret"));

	FString mAosBucket = mOssPrototype->GetValueByKey(TEXT("AosBucket"));

	// 开始初始化OSSModule.
	OSSHelper::GetInstance()->Init(mAosEndPoint, mAosAccessKeyID, mAosAccessKeySecret, mAosBucket);


	//FOssDownloadQueueParameter inDownloadParameters;
	//TArray<FOssReqeustParameter> FilePaths;
	//FilePaths.Add(FOssReqeustParameter("oss绝对路径","本地绝对路径"));
	////进度回调.
	//FOnOSSFileDownloadProgressDelegate OnOSSFileDownloadProgress(this,);
	////完成回调.
	//FOnOSSRequestFinishedDelegate OnOSSRequestFinished(this,);
	//OSSHelper::GetInstance()->GetOSSTool()->Download(inDownloadParameters);
}

FString UEditorDirector::GetServerUrl()
{
	int32 mServerPrototypeId = PrototypeConst::P_HTTP_WINDOWS_SERVER_ID;

	if (GController::m_platform == ENUM_PLATFORM::GANDROID)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_ANDROID_SERVER_ID;
	}
	else if (GController::m_platform == ENUM_PLATFORM::GIOS)
	{
		mServerPrototypeId = PrototypeConst::P_HTTP_IOS_SERVER_ID;
	}

	ServerPrototype * mServerPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ServerPrototype>(mServerPrototypeId);

	return mServerPrototype->GetValueByKey(TEXT("sharedHttpUrl"));
}

void UEditorDirector::LoadAllDataManager()
{

}

void UEditorDirector::ReleaseAllDataManager()
{

}




