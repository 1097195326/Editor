// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitPatternLibraryT.h"
#include "LoginModule.h"
#include "GameDirector.h"
#include "TSuitPatternManager.h"
#include "GModuleManager.h"

SuitPatternLibraryT *   SuitPatternLibraryT::m_pInstance = nullptr;

SuitPatternLibraryT * SuitPatternLibraryT::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new SuitPatternLibraryT();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

SuitPatternLibraryT::SuitPatternLibraryT()
{
}

SuitPatternLibraryT::~SuitPatternLibraryT()
{
}

void SuitPatternLibraryT::OnLoad()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4027, this, &SuitPatternLibraryT::OnCheckPatternExtern);

	UGameDirector::GetInstance()->RegisterInstanceClass(this);
}

void SuitPatternLibraryT::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 4027, this, &SuitPatternLibraryT::OnCheckPatternExtern);

	m_pInstance = nullptr;
}



FString SuitPatternLibraryT::GetPatternId(int32 _fabricId, const FString & _patternId)
{
	if (m_checkPatternData.Contains(_fabricId))
	{
		TMap<FString, FString> * mPatternMap = &(m_checkPatternData[_fabricId]);

		if (mPatternMap->Contains(_patternId))
		{
			return (*mPatternMap)[_patternId];
		}
	}
	return TEXT("");
}

void SuitPatternLibraryT::RequestPatternUseInFabric(int32 _fabricId, const FString & _patternId)
{
	FString mPatternId = GetPatternId(_fabricId, _patternId);

	if (!mPatternId.IsEmpty())
	{
		SendLocalMsg(EnumPatternExtern::CANT_NOT_EXTERN_PATTERN);
		return;
	}

	if (m_fabricId!=0)
	{
		return;
	}

	if (_patternId.IsEmpty())
	{
		SendLocalMsg(EnumPatternExtern::CANT_NOT_EXTERN_PATTERN);
		return;
	}

	if (_patternId.Contains(TEXT("USER")))
	{
		AddPatternData(_fabricId, _patternId);

		SendLocalMsg(EnumPatternExtern::CANT_NOT_EXTERN_PATTERN);
		return;
	}

	m_fabricId = _fabricId;

	m_patternId = _patternId;

	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	t_jsonObject->SetStringField("patternId", _patternId);

	t_jsonObject->SetNumberField("fabricId", _fabricId);

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4027, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}

void SuitPatternLibraryT::OnCheckPatternExtern(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 解析判断是否可以继承,并且保存在map中.
		TSharedPtr<FJsonObject> mjsonObject = mHttpMsg->GetHttpContent();

		bool isHas = mjsonObject->HasField(TEXT("data"));

		if (isHas)
		{
			TSharedPtr<FJsonObject> mdataObject = mjsonObject->GetObjectField(TEXT("data"));

			TSuitPatternManager::GetInstance()->EncodePatternFromCheckInFabric(mjsonObject);

			FString mPatternId = mdataObject->GetStringField(TEXT("patternId"));

 			if (m_checkPatternData.Contains(m_fabricId))
			{
				TMap<FString, FString> * mMap = &m_checkPatternData[m_fabricId];

				if (mMap->Contains(m_patternId))
				{
					(*mMap)[m_patternId] = mPatternId;
				}
				else
				{
					mMap->Add(m_patternId, mPatternId);
				}
			}
			else
			{
				TMap<FString, FString> mMap;

				mMap.Add(m_patternId, mPatternId);

				m_checkPatternData.Add(m_fabricId, mMap);
			}
		}

		SendLocalMsg(EnumPatternExtern::CAN_EXTERN_PATTERN);
	}
	else
	{
		SendLocalMsg(EnumPatternExtern::CANT_NOT_EXTERN_PATTERN);
	}
	m_fabricId = 0;
}

void SuitPatternLibraryT::SendLocalMsg(EnumPatternExtern _msgCode)
{
	// 发送一个通知.
	msg_ptr mBrodcastLoginSucess(new MsgLocal(MessageTypeLocal, 4028,&_msgCode));

	MsgCenter::Instance()->SendMsg(mBrodcastLoginSucess);
}

void SuitPatternLibraryT::AddPatternData(int32 _fabricId, const FString & _patternId)
{
	FString mPatternId = _patternId;

	if (m_checkPatternData.Contains(_fabricId))
	{
		TMap<FString, FString> * mMap = &m_checkPatternData[_fabricId];

		if (mMap->Contains(_patternId))
		{
			(*mMap)[_patternId] = mPatternId;
		}
		else
		{
			mMap->Add(_patternId, mPatternId);
		}
	}
	else
	{
		TMap<FString, FString> mMap;

		mMap.Add(_patternId, mPatternId);

		m_checkPatternData.Add(_fabricId, mMap);
	}
}

