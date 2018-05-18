

#include "HomeInfoManager.h"
#include "HttpModule.h"
#include "MainModule.h"
#include "GModuleManager.h"

HomeInfoManager *   HomeInfoManager::m_pInstance = nullptr;

HomeInfoManager * HomeInfoManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new HomeInfoManager();
	}
	return m_pInstance;
}


HomeInfoManager::HomeInfoManager()
{
	
}

HomeInfoManager::~HomeInfoManager()
{
	
}

void HomeInfoManager::OnLoad()
{
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 1099, this, &HomeInfoManager::ResponseFinish);
}

void HomeInfoManager::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 1099, this, &HomeInfoManager::ResponseFinish);
	for (HomeInfoT * info : m_BikiniTypesArr)
	{
		delete info;
	}
	for (HomeInfoT * info : m_SubTypesArr)
	{
		delete info;
	}
	m_pInstance = nullptr;

}
void HomeInfoManager::GetDataArray(TSharedPtr<FJsonObject> & _content, TArray<HomeInfoT*> & _Arr)
{
	TSharedPtr<FJsonObject> mdata = _content->GetObjectField("data");

	m_ItemTitle = mdata->GetStringField("pageName");

	TSharedPtr<FJsonObject> tModelMap = mdata->GetObjectField("tModelMap");

	TArray<FString> tModelMapKeyArray;
	tModelMap->Values.GetKeys(tModelMapKeyArray);


	for (int32 i = 0; i < tModelMapKeyArray.Num(); i++)
	{
		FString key = tModelMapKeyArray[i];
		TSharedPtr<FJsonObject> o_1 = tModelMap->GetObjectField(key);

		TSharedPtr<FJsonObject> o_2 = o_1->GetObjectField("tModelContentMap");

		TArray<FString> tModelContentMapKeyArray;
		o_2->Values.GetKeys(tModelContentMapKeyArray);

		for (int32 j = 0; j < tModelContentMapKeyArray.Num(); j++)
		{
			FString key = tModelContentMapKeyArray[j];
			TArray<TSharedPtr<FJsonValue>> o_3 = o_2->GetArrayField(key);

			for (TSharedPtr<FJsonValue> fj : o_3)
			{
				TSharedPtr<FJsonObject> _ob = fj->AsObject();
				HomeInfoT * mHomeInfo = new HomeInfoT();
				mHomeInfo->InitWithJson(_ob);

				_Arr.Add(mHomeInfo);

				FString mContent;
				TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
				FJsonSerializer::Serialize(_ob.ToSharedRef(), t_writer);
				UE_LOG(LogTemp, Log, TEXT("zhx : EncodeData : %s"), *mContent);
			}
		}
	}
}
void HomeInfoManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	for (HomeInfoT * info : m_BikiniTypesArr)
	{
		delete info;
	}
	m_BikiniTypesArr.Empty();
	GetDataArray(_content,m_BikiniTypesArr);
}
void HomeInfoManager::EncodeSportData(TSharedPtr<FJsonObject> & _content)
{
	for (HomeInfoT * info : m_SportTypesArr)
	{
		delete info;
	}
	m_SportTypesArr.Empty();
	GetDataArray(_content, m_SportTypesArr);
}
void HomeInfoManager::RequestUrl(FString _Url)
{
	for (HomeInfoT * info : m_SubTypesArr)
	{
		delete info;
	}
	m_SubTypesArr.Empty();
	m_ItemTitle = TEXT("");

	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();
	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	FString mAppendUrl = FString::Printf(TEXT("%s%s"), TEXT("cms/"),*_Url);

	msg_ptr mHttp(new MsgHttp(MessageTypeHttpRequest,1099, mToken, mOperator,false, t_jsonObject, mAppendUrl));
	
	MsgCenter::Instance()->SendMsg(mHttp);
}
void HomeInfoManager::ResponseFinish(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		FString mContent;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
		UE_LOG(LogTemp, Log, TEXT("zhx : Home Info Response Finish : %s"), *mContent);

		GetDataArray(t_jsonObject, m_SubTypesArr);

		if (m_CallbackFunc)
		{
			m_CallbackFunc(true);
		}
		
	}
	else
	{
		if (m_CallbackFunc)
		{
			m_CallbackFunc(false);
		}
	}

	m_CallbackFunc = nullptr;

}


