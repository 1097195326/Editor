// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HaoshaSDKBPLibrary.h"
#include "HaoshaSDK.h"
#include "Engine/Engine.h"
#if PLATFORM_ANDROID
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#elif PLATFORM_IOS
#include "HMWebViewManager.h"
#include "WebViewViewController.h"
#include "HMPublicTools.h"
#include "CoreDelegates.h"
#include "IOSAppDelegate.h"
#include "LLShareSDKTool.h"
#include "SystemTools.h"
#include "UncaughtExceptionHandler.h"
#endif
#include <string>
#include <iostream>

UHaoshaSDKBPLibrary::UHaoshaSDKBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UHaoshaSDKBPLibrary::HaoshaSDKSampleFunction(float Param)
{
	return -1;
}

void UHaoshaSDKBPLibrary::OpenWebview(FString _url, FString _value)
{
	FString mAnd = TEXT("?");
	if (_url.Contains(TEXT("?")))
	{
		mAnd = TEXT("&");
	}

	if (_value.IsEmpty())
	{
		mAnd = TEXT("");
	}

	FString mUrl = FString::Printf(TEXT("%s%s%s"),*_url,*mAnd,*_value);
#if PLATFORM_ANDROID
	const std::string inURL(TCHAR_TO_UTF8(*mUrl));
	
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{

			jmethodID Android_OpenURLMethodID = Env->GetStaticMethodID(Class, "Anroid_OpenWebview", "(Ljava/lang/String;)Z");
			jstring URL = Env->NewStringUTF(inURL.c_str());
		
			bool ret = Env->CallStaticBooleanMethod(Class, Android_OpenURLMethodID, URL);
			Env->DeleteLocalRef(URL);
		
		}
		Env->DeleteLocalRef(Class);
	}
#elif PLATFORM_IOS
    
    UIViewController *rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    
    NSString *nsUrl = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*mUrl)];
    NSString *nsVal = @"";
    
    dispatch_async(dispatch_get_main_queue(),^ {
        [[HMWebViewManager defaultManager] webViewInitWithcontroller:rootViewController urlString:nsUrl prama:nsVal backButtonImage:@"back" cancelButtonImage:@"close" protocolCallBack:^(NSString *protocolString)
         {
             
         }];
    } );
    
#endif
}

// 初始化分享接口.
void UHaoshaSDKBPLibrary::InitLibrary(FString _key, FString _value)
{
#if PLATFORM_ANDROID
	const std::string mKey(TCHAR_TO_UTF8(*_key));
	const std::string mValue(TCHAR_TO_UTF8(*_value));

	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_OpenURLMethodID = Env->GetStaticMethodID(Class, "Android_InitLibrary", "(Ljava/lang/String;Ljava/lang/String;)Z");
			jstring jKey = Env->NewStringUTF(mKey.c_str());
			jstring jValue = Env->NewStringUTF(mValue.c_str());
			bool ret = Env->CallStaticBooleanMethod(Class, Android_OpenURLMethodID, jKey, jValue);
			Env->DeleteLocalRef(jKey);
			Env->DeleteLocalRef(jValue);
		}
		Env->DeleteLocalRef(Class);
    }
#elif PLATFORM_IOS
    [LLShareSDKTool registerShare];
#endif
}

// 微信好友分享.
void UHaoshaSDKBPLibrary::WeChatFavorite(FString _strTitle, FString _strText, FString _imgUrl, FString _strUrl)
{
#if PLATFORM_ANDROID
	const std::string mStrTitle(TCHAR_TO_UTF8(*_strTitle));
	const std::string mStrText(TCHAR_TO_UTF8(*_strText));
	const std::string mImgUrl(TCHAR_TO_UTF8(*_imgUrl));
	const std::string mStrUrl(TCHAR_TO_UTF8(*_strUrl));
	  
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_OpenURLMethodID = Env->GetStaticMethodID(Class, "Android_WeChatFavorite", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
			jstring jStrTitle = Env->NewStringUTF(mStrTitle.c_str());
			jstring jStrText = Env->NewStringUTF(mStrText.c_str());
			jstring jImgUrl = Env->NewStringUTF(mImgUrl.c_str());
			jstring jStrUrl = Env->NewStringUTF(mStrUrl.c_str());
			bool ret = Env->CallStaticBooleanMethod(Class, Android_OpenURLMethodID, jStrTitle, jStrText, jImgUrl, jStrUrl);
			Env->DeleteLocalRef(jStrTitle);
			Env->DeleteLocalRef(jStrText);
			Env->DeleteLocalRef(jImgUrl);
			Env->DeleteLocalRef(jStrUrl);
		}
		Env->DeleteLocalRef(Class);

	}
#elif PLATFORM_IOS
    
    NSString * Title =  _strTitle.GetNSString();
    NSString * Text =  _strText.GetNSString();
    
    [[LLShareSDKTool defaultManager] shareContentWithShareContentType:HMShareContentTypeAuto
                                                    SharePlatformType:SSDKPlatformSubTypeWechatSession
                                                         contentTitle:Title
                                                   contentDescription:Text
                                                         contentImage:[UIImage imageWithData:[NSData
                                                                                              dataWithContentsOfURL:[NSURL URLWithString:
                                                                                                                     [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*_imgUrl)]]]]
                                                           contentURL:[NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*_strUrl)]
                                                              success:^{
        NSLog(@"分享成功");
    } failure:^(NSString *failureInfo) {
        NSLog(@"分享失败:%@",failureInfo);
    } OtherResponseStatus:^(SSDKResponseState state) {
        NSLog(@"分享异常类型");
    }];

#endif
}

// 微信朋友圈分享.
void UHaoshaSDKBPLibrary::WeChatMoments(FString _strTitle, FString _strText, FString _imgUrl, FString _strUrl)
{
#if PLATFORM_ANDROID
	const std::string mStrTitle(TCHAR_TO_UTF8(*_strTitle));
	const std::string mStrText(TCHAR_TO_UTF8(*_strText));
	const std::string mImgUrl(TCHAR_TO_UTF8(*_imgUrl));
	const std::string mStrUrl(TCHAR_TO_UTF8(*_strUrl));

	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_OpenURLMethodID = Env->GetStaticMethodID(Class, "Android_WeChatMoments", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
			jstring jStrTitle = Env->NewStringUTF(mStrTitle.c_str());
			jstring jStrText = Env->NewStringUTF(mStrText.c_str());
			jstring jImgUrl = Env->NewStringUTF(mImgUrl.c_str());
			jstring jStrUrl = Env->NewStringUTF(mStrUrl.c_str());
			bool ret = Env->CallStaticBooleanMethod(Class, Android_OpenURLMethodID, jStrTitle, jStrText, jImgUrl, jStrUrl);
			Env->DeleteLocalRef(jStrTitle);
			Env->DeleteLocalRef(jStrText);
			Env->DeleteLocalRef(jImgUrl);
			Env->DeleteLocalRef(jStrUrl);
		}
		Env->DeleteLocalRef(Class);

	}
#elif PLATFORM_IOS
    
    NSString * Title =  _strTitle.GetNSString();
    NSString * Text =  _strText.GetNSString();
    
    [[LLShareSDKTool defaultManager] shareContentWithShareContentType:HMShareContentTypeAuto
                                                    SharePlatformType:SSDKPlatformSubTypeWechatTimeline
                                                         contentTitle:Title
                                                   contentDescription:Text
                                                         contentImage:[UIImage imageWithData:[NSData
                                                                                              dataWithContentsOfURL:[NSURL URLWithString:
                                                                                                                     [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*_imgUrl)]]]]
                                                           contentURL:[NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*_strUrl)]
                                                              success:^{

        NSLog(@"分享成功");
    } failure:^(NSString *failureInfo) {

        NSLog(@"分享失败:%@",failureInfo);

    } OtherResponseStatus:^(SSDKResponseState state) {
        NSLog(@"分享异常类型");
    }];

#endif
}

void UHaoshaSDKBPLibrary::ConvertToJPG(FString _pngFilePath, FString _jpgFilePath)
{
#if PLATFORM_ANDROID
	const std::string mPngFilePath(TCHAR_TO_UTF8(*_pngFilePath));
	const std::string mJpgFilePath(TCHAR_TO_UTF8(*_jpgFilePath));

	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_OpenURLMethodID = Env->GetStaticMethodID(Class, "Android_ConvertToJPG", "(Ljava/lang/String;Ljava/lang/String;)Z");
			jstring jPngFilePath = Env->NewStringUTF(mPngFilePath.c_str());
			jstring jJpgFilePath = Env->NewStringUTF(mJpgFilePath.c_str());
			bool ret = Env->CallStaticBooleanMethod(Class, Android_OpenURLMethodID, jPngFilePath, jJpgFilePath);
			Env->DeleteLocalRef(jPngFilePath);
			Env->DeleteLocalRef(jJpgFilePath);
		}
		Env->DeleteLocalRef(Class);
	}
#endif
}

void UHaoshaSDKBPLibrary::OpenURL(FString _inURL)
{
#if PLATFORM_ANDROID
	const std::string inURL(TCHAR_TO_UTF8(*_inURL));
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_OpenURLMethodID = Env->GetStaticMethodID(Class, "Android_OpenLocalWeb", "(Ljava/lang/String;)Z");
			jstring jInURL = Env->NewStringUTF(inURL.c_str());
			bool ret = Env->CallStaticBooleanMethod(Class, Android_OpenURLMethodID, jInURL);
			Env->DeleteLocalRef(jInURL);
		}
		Env->DeleteLocalRef(Class);
	}
#elif PLATFORM_IOS
    NSString *nsUrl = [NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*_inURL)];
  
	[HMPublicTools openAppStoreWithUrl : nsUrl];
#endif
}

void UHaoshaSDKBPLibrary::FlipImage(FString _inFilePath, FString _outFilePath)
{
#if PLATFORM_ANDROID
	const std::string inFilePath(TCHAR_TO_UTF8(*_inFilePath));
	const std::string outFilePath(TCHAR_TO_UTF8(*_outFilePath));
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_FlipImageMethodID = Env->GetStaticMethodID(Class, "Android_FlipImage", "(Ljava/lang/String;Ljava/lang/String;)Z");
			jstring jInURL = Env->NewStringUTF(inFilePath.c_str());
			jstring jOutURL = Env->NewStringUTF(outFilePath.c_str());
			bool ret = Env->CallStaticBooleanMethod(Class, Android_FlipImageMethodID, jInURL, jOutURL);
			Env->DeleteLocalRef(jInURL);
			Env->DeleteLocalRef(jOutURL);
		}
		Env->DeleteLocalRef(Class);
	}

#endif
}
FString UHaoshaSDKBPLibrary::GetDeviceType()
{
#if PLATFORM_ANDROID
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_GetSystemModelID = Env->GetStaticMethodID(Class, "Android_GetSystemModel", "()Ljava/lang/String;");
			jstring JSystemModelName = (jstring)Env->CallStaticObjectMethod(Class, Android_GetSystemModelID);
			const char * CharModelName = Env->GetStringUTFChars(JSystemModelName, 0);
			FString ModelName = FString(CharModelName);
			Env->ReleaseStringUTFChars(JSystemModelName, CharModelName);
			Env->DeleteLocalRef(JSystemModelName);
			Env->DeleteLocalRef(Class);
			return ModelName;
		}
	}
	return TEXT("");
#elif PLATFORM_IOS
	return UTF8_TO_TCHAR([[SystemTools deviceVersion] UTF8String]);
#endif
	return TEXT("");
}

FString UHaoshaSDKBPLibrary::GetDeviceVersion()
{
#if PLATFORM_ANDROID
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_GetSystemVersionID = Env->GetStaticMethodID(Class, "Android_GetSystemVersion", "()Ljava/lang/String;");
			jstring JName = (jstring)Env->CallStaticObjectMethod(Class, Android_GetSystemVersionID);
			const char * CharName = Env->GetStringUTFChars(JName, 0);
			FString fName = FString(CharName);
			Env->ReleaseStringUTFChars(JName, CharName);
			Env->DeleteLocalRef(JName);
			Env->DeleteLocalRef(Class);
			return fName;
		}
}
	return TEXT("");
#elif PLATFORM_IOS
	return UTF8_TO_TCHAR([[SystemTools iOSVersion] UTF8String]);
#endif
	return TEXT("");
}

void UHaoshaSDKBPLibrary::HaosaInstallUncaughtExceptionHandler()
{
#if PLATFORM_ANDROID
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_InitErrorInfoDesginID = Env->GetStaticMethodID(Class, "Android_InitErrorInfoDesgin", "()V");
			Env->CallStaticVoidMethod(Class, Android_InitErrorInfoDesginID);
			Env->DeleteLocalRef(Class);
		}
	}
#elif PLATFORM_IOS
    InstallUncaughtExceptionHandler();
#endif
}

void UHaoshaSDKBPLibrary::InitExceptionHandler(FString _appVersion, FString _appAcount, FString _apiVersion)
{
    UE_LOG(LogTemp,Log,TEXT("zhx : _appVersion = %s, _appAcount = %s, _apiVersion = %s"), *_appVersion, *_appAcount, *_apiVersion);
#if PLATFORM_ANDROID
	const std::string mAppVersion(TCHAR_TO_UTF8(*_appVersion));
	const std::string mAppAcount(TCHAR_TO_UTF8(*_appAcount));
	const std::string mApiVersion(TCHAR_TO_UTF8(*_apiVersion));
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_InitExceptionID = Env->GetStaticMethodID(Class, "Android_InitException", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			jstring jAppVersion = Env->NewStringUTF(mAppVersion.c_str());
			jstring jAppAcount = Env->NewStringUTF(mAppAcount.c_str());
			jstring jApiVersion = Env->NewStringUTF(mApiVersion.c_str());
			Env->CallStaticVoidMethod(Class, Android_InitExceptionID, jAppVersion, jAppAcount, jApiVersion);
			Env->DeleteLocalRef(jAppVersion);
			Env->DeleteLocalRef(jAppAcount);
			Env->DeleteLocalRef(jApiVersion);
		}
		Env->DeleteLocalRef(Class);
	}
#elif PLATFORM_IOS
	[SystemTools setUserID : [NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(*_appAcount)] setApiVersion:[NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*_apiVersion)] setAppVersion:[NSString stringWithFormat:@"%s",TCHAR_TO_UTF8(*_appVersion)]];
#endif
}

FString UHaoshaSDKBPLibrary::GetErrorLog()
{
#if PLATFORM_ANDROID
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_GetErrorInfoDesginID = Env->GetStaticMethodID(Class, "Android_GetErrorInfoDesgin", "()Ljava/lang/String;");
			jstring JName = (jstring)Env->CallStaticObjectMethod(Class, Android_GetErrorInfoDesginID);
			const char * CharName = Env->GetStringUTFChars(JName, 0);
			FString fName = FString(CharName);
			Env->ReleaseStringUTFChars(JName, CharName);
			Env->DeleteLocalRef(JName);
			Env->DeleteLocalRef(Class);
			UE_LOG(LogTemp, Log, TEXT("zhoujia :GetErrorInfoDesgin  %s"), *fName);

			return fName;
		}
	}
	return TEXT("");
#elif PLATFORM_IOS
	NSString * mNsStr = [SystemTools getErrorLog];

	FString mDownloadPath = UTF8_TO_TCHAR([mNsStr UTF8String]);

	return mDownloadPath;
#endif
	return TEXT("");
}

void UHaoshaSDKBPLibrary::ApplicationRegisteredForRemoteNotificationsDelegate_Handler(TArray<uint8> inArray)
{
#if PLATFORM_ANDROID

#elif PLATFORM_IOS
	//const std::string cstr(reinterpret_cast<const char*>(inArray), inArray.Num());

	//FString mToken(cstr.c_str());

	//NSString *token = [[NSString stringWithFormat : @"%s",TCHAR_TO_UTF8(*mToken)] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
	//	token = [token stringByReplacingOccurrencesOfString : @" " withString:@""];
	//	//NSLog(@"\n>>>[DeviceToken Success]:%@\n\n", token);

	//	//    [[NSNotificationCenter defaultCenter]postNotificationName:@"ceshi" object:nil userInfo:@{@"token":token}];

	//	// 向个推服务器注册deviceToken
	//	[GeTuiSdk registerDeviceToken : token];
#endif
}

void UHaoshaSDKBPLibrary::DeleteExceptionInfo()
{
#if PLATFORM_ANDROID
	JNIEnv *Env = FAndroidApplication::GetJavaEnv();
	if (Env)
	{
		jclass Class = FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity");
		if (nullptr != Class)
		{
			jmethodID Android_CleanErrorInfoDesginID = Env->GetStaticMethodID(Class, "Android_CleanErrorInfoDesgin", "()V");
			Env->CallStaticVoidMethod(Class, Android_CleanErrorInfoDesginID);
			Env->DeleteLocalRef(Class);
		}
	}

#elif PLATFORM_IOS
	[SystemTools cleanErrorLog];
#endif
}
void UHaoshaSDKBPLibrary::InitNotification()
{
#if PLATFORM_ANDROID

#elif PLATFORM_IOS
	//NSDictionary* launchDict = [IOSAppDelegate GetDelegate].launchOptions;

	//[GeTuiSdk startSdkWithAppId : kGtAppId appKey : kGtAppKey appSecret : kGtAppSecret delegate : [IOSAppDelegate GetDelegate]];

	//[self registerRemoteNotification];
	//[GeTuiSdk runBackgroundEnable : true];
	//[GeTuiSdk setChannelId : @"GT-Channel"];

	//NSInteger page = 0;
	//[GeTuiSdk resetBadge];
	//[[UIApplication sharedApplication] setApplicationIconBadgeNumber:page];
	//[[UIApplication sharedApplication] cancelAllLocalNotifications];
#endif
}

