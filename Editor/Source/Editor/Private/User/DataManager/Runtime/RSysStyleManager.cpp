// Fill out your copyright notice in the Description page of Project Settings.

#include "RSysStyleManager.h"
#include "MsgCenter.h"
#include "Editor.h"
#include "GModuleManager.h"
#include "LoginModule.h"

RSysStyleManager *   RSysStyleManager::m_pInstance = nullptr;

RSysStyleManager * RSysStyleManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RSysStyleManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RSysStyleManager::RSysStyleManager()
{
}

RSysStyleManager::~RSysStyleManager()
{
}  

void RSysStyleManager::OnLoad()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4001, this, &RSysStyleManager::OnLoadStyleFinish);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 1022, this, &RSysStyleManager::OnLoadStyleByIdFinish);
}

void RSysStyleManager::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 4001, this, &RSysStyleManager::OnLoadStyleFinish);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 1022, this, &RSysStyleManager::OnLoadStyleByIdFinish);

	m_pInstance = nullptr;
}

void RSysStyleManager::OnLoadStyleByIdFinish(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mJsonData = t_jsonObject->GetObjectField("data");

		FString mContent;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
		UE_LOG(LogTemp, Log, TEXT("zhx : OnLoadStyleByIdFinish : %s"), *mContent);

		// 3.解析数据.
		EncodeStyleDataById(mJsonData, mArrayRuntimeData);

		if (m_loadFinishDelegate)
			m_loadFinishDelegate(0, 0, 0, 0, mArrayRuntimeData);
	}
	else
	{
		// 通知代理加载数据异常.
		if (m_loadErrorDelegate)
			m_loadErrorDelegate(mHttpMsg->GetServerMsg());
	}

	ClearBind();
}
void RSysStyleManager::OnLoadStyleFinish(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mJsonData = t_jsonObject->GetObjectField("data");

		FString mContent;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
		UE_LOG(LogTemp, Log, TEXT("zhx : OnLoadStyleFinish : %s"), *mContent);

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
		// 通知代理加载数据异常.
		if (m_loadErrorDelegate)
			m_loadErrorDelegate(mHttpMsg->GetServerMsg());
	}

	//ClearBind();
}

void RSysStyleManager::LoadStyle(int32 _pageIndex, int32 _pageSize,TArray<int32> _catIds)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetNumberField("pageIndex", _pageIndex);

	t_jsonObject->SetNumberField("pageSize", _pageSize);

	/*if(_catId != 0)
		t_jsonObject->SetNumberField("catId", _catId);*/

	if (_catIds.Num() > 0)
	{
		TArray<TSharedPtr<FJsonValue>> CatIdArr;
		for (int32 mCatId : _catIds)
		{
			CatIdArr.Add(MakeShareable(new FJsonValueNumber(mCatId)));
		}
		t_jsonObject->SetArrayField("catIds", CatIdArr);
	}


	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();
	 
	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4001, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}


void RSysStyleManager::LoadStyleById(int32 _id)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetNumberField("styleId", _id); // 10297

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 1022, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}
