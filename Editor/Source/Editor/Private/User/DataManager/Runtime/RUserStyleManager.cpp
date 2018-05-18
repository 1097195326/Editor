// Fill out your copyright notice in the Description page of Project Settings.

#include "RUserStyleManager.h"
#include "MsgCenter.h"
#include "Editor.h"
#include "GModuleManager.h"
#include "StyleR.h"
#include "LoginModule.h"

RUserStyleManager *   RUserStyleManager::m_pInstance = nullptr;

RUserStyleManager * RUserStyleManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RUserStyleManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RUserStyleManager::RUserStyleManager()
{
}

RUserStyleManager::~RUserStyleManager()
{
}

void RUserStyleManager::OnLoad()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4002, this, &RUserStyleManager::OnLoadStyleFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4102, this, &RUserStyleManager::OnLoadStyleFinishForMainUI);
}

void RUserStyleManager::OnRelease()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4002, this, &RUserStyleManager::OnLoadStyleFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4102, this, &RUserStyleManager::OnLoadStyleFinishForMainUI);

	m_pInstance = nullptr;
}


void RUserStyleManager::LoadStyle(int32 _pageIndex, int32 _pageSize)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetNumberField("pageIndex", _pageIndex);

	t_jsonObject->SetNumberField("pageSize", _pageSize);

	//t_jsonObject->SetStringField("ownerId", mLoginModule->GetUserData()->GetUserId());

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4002, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}

void RUserStyleManager::LoadStyleForMainUI(int32 _pageIndex, int32 _pageSize)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetNumberField("pageIndex", _pageIndex);

	t_jsonObject->SetNumberField("pageSize", _pageSize);

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4102, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}

void RUserStyleManager::OnLoadStyleFinish(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mJsonData = t_jsonObject->GetObjectField("data");

		// 获取内容. 
		int32 mPageIndex = mJsonData->GetNumberField("pageIndex");

		int32 mPageSize = mJsonData->GetNumberField("pageSize");

		int32 mRecordCount = mJsonData->GetNumberField("recordCount");

		int32 mTotalPage = mJsonData->GetNumberField("totalPage");

		// 3.解析数据.
		EncodeStyleData(mJsonData, mArrayRuntimeData);

		// 4.通知代理已下载完成.
		if (m_loadFinishDelegate)
			m_loadFinishDelegate(mPageIndex, mPageSize, mRecordCount, mTotalPage, mArrayRuntimeData);

		ClearBind();
	}
	else
	{
		// 获取异常内容.	
		if (m_loadErrorDelegate)
			m_loadErrorDelegate(mHttpMsg->GetServerMsg());
	}
	//ClearBind();
}

void RUserStyleManager::OnLoadStyleFinishForMainUI(msg_ptr _msg)
{
	LoadUserStyleData mUserStyleData;

	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mJsonData = t_jsonObject->GetObjectField("data");

		// 获取内容. 
		mUserStyleData._pageIndex = mJsonData->GetNumberField("pageIndex");

		mUserStyleData._pageSize = mJsonData->GetNumberField("pageSize");

		mUserStyleData._recordCount = mJsonData->GetNumberField("recordCount");

		mUserStyleData._totalPage = mJsonData->GetNumberField("totalPage");

		// 3.解析数据.
		EncodeStyleData(mJsonData, mArrayRuntimeData);

		mUserStyleData._ArrayRuntimData = mArrayRuntimeData;

		mUserStyleData._msgSuccess = true;

		// 4.通知代理已下载完成.
		msg_ptr mLocalMsg(new MsgLocal(MessageTypeLocal, 4102, &mUserStyleData));

		MsgCenter::Instance()->SendMsg(mLocalMsg);
	}
	else
	{
		mUserStyleData._msgSuccess = false;

		msg_ptr mLocalMsg(new MsgLocal(MessageTypeLocal, 4102, &mUserStyleData));

		MsgCenter::Instance()->SendMsg(mLocalMsg);
	}
}
