// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MsgCenter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/TextProperty.h"
#include "Engine/GameInstance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GCore/Public/System/GameDirector.h"
#include "EditorDirector.generated.h"

/**
 * 
 */
UCLASS() 
class EDITOR_API UEditorDirector : public UGameDirector
{
	GENERATED_BODY()
	
public:

	virtual void OnLoad() override;

	
	virtual void OnRelease() override;
    
    virtual void PostInitProperties() override;
    
    virtual void BeginDestroy() override;

	
	UFUNCTION(Exec)
		void LoadXmlFile();
	UFUNCTION(Exec)
		void PakMount();

	bool MountLocalPakFiles(const FString &inFolder);

	// 初始化异常.
	void InitExption();

public:
	// 初始化OSS.
	void InitOSS();

	FString GetServerUrl();
private:
	// 初始化HttpManager.
	void InitHttpManager();
    
    // 获取异常内容,并发送到服务器上.
    void SendErrorMsgToServer();
    
    // AppDelegateNotification.
    void ApplicationRegisteredForRemoteNotificationsDelegate_Handler(TArray<uint8> inArray);

	// 加载所有的数据管理器.
	void LoadAllDataManager();

	// 释放所有的数据管理器.
	void ReleaseAllDataManager();
    
    // 收到服务器回调.
    void OnErrorResponse(msg_ptr _msg);

	// 异步接受异常消息并发送给服务器.
	void SendErrorMsgThread();
};
