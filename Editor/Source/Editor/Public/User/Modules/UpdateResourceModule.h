// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/BaseData/BaseModule.h"
#include "MsgCenter.h"


/**
 * 
 */
class EDITOR_API UpdateResourceModule : public BaseModule
{
	
public:

	UpdateResourceModule();
	virtual ~UpdateResourceModule();

	virtual void OnLoad() override;
	virtual void OnRelease() override;


	// 往服务器发送消息的区域.
	void RequestForcedUpdateServer();
	void OnForcedUpdateResponseMsg(msg_ptr _msg);

	void TestHotUpdate();

	bool Tick(float DeltaTime);
	//开始热更新.
	void StartHotUpdate(msg_ptr _msg);
	//更新完成通知logicScene跳转到下个level.
	void HotUpdateFinished();

public:
	int32 m_ForceUpdate_Response_LocalPid;

	int32 m_HotUpdate_StartDownload_LocalPid;
	int32 m_HotUpdate_DownloadSize_LocalPid;
	int32 m_HotUpdate_DownloadSpeed_LocalPid;
	int32 m_HotUpdate_DownloadProgress_LocalPid;
	int32 m_HotUpdate_Finished_LocalPid;
	
	FTickerDelegate TickDelegate;      
	FDelegateHandle TickDelegateHandle;
	
};
