// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitPatternManager.h"
#include "SuitPatternT.h"
#include "LoginModule.h"
#include "GModuleManager.h"


TSuitPatternManager *   TSuitPatternManager::m_pInstance = nullptr;

TSuitPatternManager * TSuitPatternManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitPatternManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitPatternManager::TSuitPatternManager()
{
}

TSuitPatternManager::~TSuitPatternManager()
{
}

void TSuitPatternManager::OnLoad()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4024, this, &TSuitPatternManager::OnLoadPatternFinish);
}

void TSuitPatternManager::OnRelease()
{
	m_pInstance = nullptr;
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4024, this, &TSuitPatternManager::OnLoadPatternFinish);

	for (auto Iter = m_patternMgr.CreateIterator(); Iter; ++Iter)
	{
		SuitPatternT * mAlertUI = Iter->Value;

		// 删除掉.
		mAlertUI->GRelease();
	}

	m_patternMgr.Empty();
}

void TSuitPatternManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitPatternT * mData = new SuitPatternT();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		//AddData(mData);
		AddPatternData(mData->m_patternId, mData);
	}
}

void TSuitPatternManager::AddPatternData(const FString & _key, SuitPatternT * _suitPatternT)
{
	if (m_patternMgr.Contains(_key))
	{
		_suitPatternT->GRelease();
		return;
		/*SuitPatternT * mPatternT = m_patternMgr[_key];

		mPatternT->GRelease();

		m_patternMgr.Remove(_key);*/
	}
	m_patternMgr.Add(_key, _suitPatternT);
}

void TSuitPatternManager::OnLoadPatternFinish(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	LoadPatternStruct mStruct;

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mJsonData = t_jsonObject->GetObjectField("data");

		// 获取内容. 
		mStruct.m_pageIndex = mJsonData->GetNumberField(TEXT("pageIndex"));

		mStruct.m_pageSize = mJsonData->GetNumberField(TEXT("pageSize"));

		mStruct.m_recordCount = mJsonData->GetNumberField(TEXT("recordCount"));

		mStruct.m_totalPage = mJsonData->GetNumberField(TEXT("totalPage"));

		// 3.解析数据.
		TArray<TSharedPtr<FJsonValue>> mJsonArray = mJsonData->GetArrayField(TEXT("content"));

		for (int32 i=0;i<mJsonArray.Num();i++)
		{
			TSharedPtr<FJsonValue> mJsonValue = mJsonArray[i];

			TSharedPtr<FJsonObject> mPatternJson = mJsonValue->AsObject();

			SuitPatternT * mData = new SuitPatternT();

			mData->GLoad();

			mData->InitWithJson(mPatternJson);

			// 添加到容器里.
			if (m_patternMgr.Contains(mData->m_patternId))
			{
				mStruct.m_patternArray.Add(m_patternMgr[mData->m_patternId]);
				mData->GRelease();
			}
			else
			{
				m_patternMgr.Add(mData->m_patternId, mData);
				mStruct.m_patternArray.Add(mData);
			}
			
		}

		// 4.发送通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_downloadPatternEvent,&mStruct));

		MsgCenter::Instance()->SendMsg(mMsg);

	}
	else
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_downloadPatternEvent, &mStruct));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

SuitPatternT * TSuitPatternManager::GetPatternData(const FString & _key)
{
	if (m_patternMgr.Contains(_key))
	{
		SuitPatternT * mPatternT = m_patternMgr[_key];

		return mPatternT;
	}
	return nullptr;
}

TArray<SuitPatternT*> TSuitPatternManager::GetPatternArray(int32 _pageIndex)
{
	m_patternArray.Empty();

	// 开始index.   
	int32 mStartIndex = (_pageIndex - 1)*m_getPatternCount;

	int32 mEndIndex = mStartIndex + m_getPatternCount;

	TArray<FString> mKeyArray;

	for (auto It = m_patternMgr.CreateConstIterator(); It; ++It)
	{
		mKeyArray.Add(It.Key());
	}

	for (int32 j = 0; j < mEndIndex; ++j)
	{
		m_patternArray.Add(GetPatternData(mKeyArray[j]));
	}

	return m_patternArray;
}

void TSuitPatternManager::GetPatternByIndex(int32 _pageIndex, int32 _pageSize,TArray<FString> & _patternIds, int32 _fabricId/*=0*/,const FString & _sn /*= TEXT("")*/, const FString & _ownerType /*= TEXT("")*/)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField(TEXT("ownerId"), mLoginModule->GetUserData()->GetUserId()); // 10297
	t_jsonObject->SetStringField(TEXT("sn"), _sn); // 10297
	t_jsonObject->SetStringField(TEXT("ownerType"), TEXT("")); // 10297
	t_jsonObject->SetNumberField(TEXT("pageIndex"),_pageIndex);
	t_jsonObject->SetNumberField(TEXT("pageSize"), _pageSize);
	t_jsonObject->SetNumberField(TEXT("fabricId"), _fabricId);

	TArray<TSharedPtr<FJsonValue>> mPatternArray;
	for (int32 i=0;i<_patternIds.Num();i++)
	{
		TSharedPtr<FJsonValue> EntryValue = MakeShareable(new FJsonValueString(*_patternIds[i]));

		mPatternArray.Add(EntryValue);
	}

	t_jsonObject->SetArrayField(TEXT("ids"), mPatternArray);

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4024, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}

void TSuitPatternManager::EncodePatternFromStyleDetailData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField(TEXT("stylePattern"));

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitPatternT * mData = new SuitPatternT();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		//AddData(mData);
		AddPatternData(mData->m_patternId, mData);
	}
}

void TSuitPatternManager::EncodePatternFromCheckInFabric(TSharedPtr<FJsonObject> & _content)
{
	TSharedPtr<FJsonObject> mJsonData = _content->GetObjectField("data");

	SuitPatternT * mData = new SuitPatternT();

	mData->GLoad();

	mData->InitWithJson(mJsonData);

	// 添加到容器里.
	//AddData(mData);
	AddPatternData(mData->m_patternId, mData);
}

