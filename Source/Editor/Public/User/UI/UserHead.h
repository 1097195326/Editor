// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PersonInfo.h"
#include "LoginModule.h"
#include "LogicMainScene.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "UserHead.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UUserHead : public UBaseUI
{
	GENERATED_BODY()
	
private:

public:
	UUserHead(const FObjectInitializer& ObjectInitializer);

	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	// 打开设置.
	void OpenUserHead(int32 _pid);

	//	收到服务器发送给本地获取头像昵称成功的消息.
	void OnGetUserHeadResponseMsg(msg_ptr _msg);

	// 图片异步下载完成的回调.
	void OnDownloadImage(bool _isScuccess,int32 _penetrateId, FString _url, UTexture2D * _texture2d,UDownloadImageTool * _downloadTool);
	
protected:
	UButton* m_BtnUserHead;	//	用户头像按钮.

	UImage* m_ImageUserHead;	//	用户头像图片.

	UDownloadImageTool *m_DownloadImageTool = nullptr;
};
