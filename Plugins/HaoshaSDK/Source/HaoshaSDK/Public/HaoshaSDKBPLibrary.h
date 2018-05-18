// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "HaoshaSDKBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class HAOSHASDK_API UHaoshaSDKBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "HaoshaSDK sample test testing"), Category = "HaoshaSDKTesting")
	static float HaoshaSDKSampleFunction(float Param);

	UFUNCTION(BlueprintCallable, Category = "OpenWebview")
	static void OpenWebview(FString _url, FString _value = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "InitLibrary")
	static void InitLibrary(FString _key, FString _value);

	//// 微信好友分享..
	UFUNCTION(BlueprintCallable, Category = "WechatFavorite")
	static void WeChatFavorite(FString _strTitle, FString _strText, FString _imgUrl, FString _strUrl);

	//// 微信圈友分享..
	UFUNCTION(BlueprintCallable, Category = "WechatFavorite")
	static void WeChatMoments(FString _strTitle, FString _strText, FString _imgUrl, FString _strUrl);


	//// 转换jpg..
	UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
	static void ConvertToJPG(FString _pngFilePath,FString _jpgFilePath);

	//// 本地浏览器打开URl..
	UFUNCTION(BlueprintCallable, Category = "OpenURL")
	static void OpenURL(FString _inURL);

	//// 水平翻转图片..
	UFUNCTION(BlueprintCallable, Category = "OpenURL")
	static void FlipImage(FString _inFilePath, FString _outFilePath);

	//// 获取设备型号..
	UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
	static FString GetDeviceType();

	//// 获取设备版本号..
	UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
	static FString GetDeviceVersion();

    // 初始化异常捕捉.
    UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
    static void HaosaInstallUncaughtExceptionHandler();

	// 初始化异常捕捉的变量.
	UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
	static void InitExceptionHandler(FString _appVersion,FString _appAcount,FString _apiVersion);

	// 获取错误信息.
	UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
	static FString GetErrorLog();

	// 初始化推送.
	UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
	static void InitNotification();
    
    
    // 初始化推送.
    UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
    static void DeleteExceptionInfo();
    
    UFUNCTION(BlueprintCallable, Category = "ConvertToJPG")
    static void ApplicationRegisteredForRemoteNotificationsDelegate_Handler(TArray<uint8> inArray);
};
