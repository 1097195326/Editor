// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdateResourceModule.h"
#include "ServerPrototype.h"
#include "PrototypeManager.h"
#include "MsgCenter.h"
#include "UIManager.h"
#include "GController.h"
#include "Editor.h"



// 反射.
GREGISTER_MODULE(UpdateResourceModule)
float Progress;

UpdateResourceModule::UpdateResourceModule()
{
	m_ForceUpdate_Response_LocalPid = 3002;
	m_HotUpdate_StartDownload_LocalPid = 3003;
	m_HotUpdate_DownloadSize_LocalPid= 3004;
	m_HotUpdate_DownloadSpeed_LocalPid=3005;
	m_HotUpdate_DownloadProgress_LocalPid=3006;
	m_HotUpdate_Finished_LocalPid = 3008;
}

UpdateResourceModule::~UpdateResourceModule()
{

}

void UpdateResourceModule::OnLoad()
{
	// 注册监听消息.
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 3001,this, &UpdateResourceModule::OnForcedUpdateResponseMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, m_HotUpdate_StartDownload_LocalPid, this, &UpdateResourceModule::StartHotUpdate);

	
}

void UpdateResourceModule::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 3001, this, &UpdateResourceModule::OnForcedUpdateResponseMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, m_HotUpdate_StartDownload_LocalPid, this, &UpdateResourceModule::StartHotUpdate);
	Progress = 0;
}
//请求服务器.
void UpdateResourceModule::RequestForcedUpdateServer()
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

	ServerPrototype * mOssPrototype = PrototypeManager::GetInstance()->GetPrototypeByPID<ServerPrototype>(mServerPrototypeId);


	// 1.打包发送消息的内容json格式.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);
	FString mOS = mOssPrototype->GetValueByKey(TEXT("platform"));
	FString mVersion = GController::m_appVersion;
	t_jsonObject->SetStringField(TEXT("os"), *mOS);
	t_jsonObject->SetStringField(TEXT("version"), *mVersion);


	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	// 不需要登录token可以为空.
	FString mToken("token");
	FString mOperator("operator");
	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 3001, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}

void UpdateResourceModule::OnForcedUpdateResponseMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
	
		// 2.消息正常,获取内容.
 		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		// 本地广播.
		msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, m_ForceUpdate_Response_LocalPid , &t_jsonObject));
		MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);
		
	}
	else
	{
		// 消息异常,自行根据逻辑判断是否重发.

		// 获取异常内容.
		FString mErrorInfo = mHttpMsg->GetMsgErrorInfo();

		UE_LOG(GEditorLog, Error, TEXT("http response Error %s"), *mErrorInfo);
	}
}

void UpdateResourceModule::TestHotUpdate()
{
	TickDelegate = FTickerDelegate::CreateRaw(this, &UpdateResourceModule::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
	
}

bool UpdateResourceModule::Tick(float DeltaTime)
{
	if (Progress<100)
	{
		Progress += 1;
		TSharedPtr<FJsonObject> t_jsonObject=MakeShareable(new FJsonObject);;
		t_jsonObject->SetStringField(TEXT("DownloadProgress"), FString::Printf(TEXT("%f"), Progress));

		// 本地广播.
		msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, m_HotUpdate_DownloadProgress_LocalPid, &t_jsonObject));
		MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);

	}
	else
	{
		FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
		HotUpdateFinished();
	}
	
	return true;
}

void UpdateResourceModule::StartHotUpdate(msg_ptr _msg)
{
	TestHotUpdate();
}

void UpdateResourceModule::HotUpdateFinished()
{
	// 本地广播.
	msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, m_HotUpdate_Finished_LocalPid,nullptr));
	MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);
}

