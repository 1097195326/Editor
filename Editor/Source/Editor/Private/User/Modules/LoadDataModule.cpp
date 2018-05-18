// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadDataModule.h"
#include "LoginModule.h"
#include "StyleR.h"
#include "GModuleManager.h"
#include "TSuitFabricLibraryManager.h"
#include "TSuitAccessoriesManager.h"
#include "TSuitComponentManager.h"
#include "TSuitFabricColorLibraryManager.h"
#include "TSuitFabricColorManager.h"
#include "TSuitFabricLibraryManager.h"
#include "TSuitFabricManager.h"
#include "TSuitPartManager.h"
#include "TSuitPatternManager.h"
#include "TSuitSliceManager.h"
#include "TSuitStitchManager.h"
#include "TSuitManager.h"
#include "UpdateUIController.h"
#include "RStyleAccessoriesManager.h"
#include "GLanguageTool.h"
#include "RStyleComponentManager.h"
#include "RStyleFabricColorManager.h"
#include "LogicUpdateScene.h"
#include "RStyleFabricManager.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "RStylePartManager.h"
#include "RStylePatternManager.h"
#include "RStyleSliceManager.h"
#include "RStyleStitchManager.h"
#include "RSysStyleManager.h"
#include "RUserStyleManager.h"
#include "TDesignTypeManager.h"
#include "HomeInfoManager.h"
#include "Editor.h"
#include "GFileHelper.h"
#include "GPlatformFileManager.h"

// 反射.
GREGISTER_MODULE(LoadDataModule)

LoadDataModule::LoadDataModule()
{
}

LoadDataModule::~LoadDataModule()
{
}

void LoadDataModule::OnLoad()
{
	// 设置一下本地保存xml的路径.
	FString mPresentPath = GFileHelper::ProjectPersistentDownloadDir();

	m_localSaveXmlPath = FString::Printf(TEXT("%s/Download/Xml/XmlVersion.xml"),*mPresentPath);

	if (!GPlatformFileManager::FileExists(*m_localSaveXmlPath))
	{
		GPlatformFileManager::RecursiveCreateDirectory(*m_localSaveXmlPath);
	}
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, m_loadStyleRDetailFinish, this, &LoadDataModule::OnLoadStyleDetailRMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, m_loadDesignTypeFinish, this, &LoadDataModule::OnLoadDesignTypeTMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, m_loadHomeInfoBikiniFinish, this, &LoadDataModule::OnLoadHomeInfoBikiniTMsg);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, m_loadHomeInfoSportFinish, this, &LoadDataModule::OnLoadHomeInfoSportTMsg);

	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, m_loadUserPattern, this, &LoadDataModule::OnLoadUserPatternMsg);
	
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4018, this, &LoadDataModule::OnGetXmlUrl);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4019, this, &LoadDataModule::OnGetXmlData);
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeHttpResponse, 4021, this, &LoadDataModule::OnGetXmlContent);
}

void LoadDataModule::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 4021, this, &LoadDataModule::OnGetXmlContent);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 4018, this, &LoadDataModule::OnGetXmlUrl);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, 4019, this, &LoadDataModule::OnGetXmlData);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, m_loadUserPattern, this, &LoadDataModule::OnLoadUserPatternMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, m_loadStyleRDetailFinish, this, &LoadDataModule::OnLoadStyleDetailRMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, m_loadDesignTypeFinish, this, &LoadDataModule::OnLoadDesignTypeTMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, m_loadHomeInfoBikiniFinish, this, &LoadDataModule::OnLoadHomeInfoBikiniTMsg);
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeHttpResponse, m_loadHomeInfoSportFinish, this, &LoadDataModule::OnLoadHomeInfoSportTMsg);

	for (int32 i=0;i<m_localXmlDownloadStructArray.Num();i++)
	{
		delete m_localXmlDownloadStructArray[i];
	}

	for (int32 i = 0; i < m_serverXmlDownloadStructArray.Num(); i++)
	{
		delete m_serverXmlDownloadStructArray[i];
	}
	m_serverXmlDownloadStructArray.Empty();

	m_localXmlDownloadStructArray.Empty();
}

void LoadDataModule::OnLoadSuitTMsg(TSharedPtr<FJsonObject> & mJsonObject)
{
	TSuitManager::GetInstance()->EncodeData(mJsonObject);
}

void LoadDataModule::OnLoadFabricLibraryTMsg(TSharedPtr<FJsonObject> & mJsonObject)
{
	TSuitFabricLibraryManager::GetInstance()->EncodeData(mJsonObject);
}

void LoadDataModule::OnLoadFabricTMsg(TSharedPtr<FJsonObject> & t_jsonObject)
{
	TSuitFabricManager::GetInstance()->EncodeData(t_jsonObject);
}

void LoadDataModule::OnLoadFabricColorTMsg(TSharedPtr<FJsonObject> & t_jsonObject)
{
	TSuitFabricColorLibraryManager::GetInstance()->EncodeData(t_jsonObject);
}


void LoadDataModule::OnLoadPatternTMsg(TSharedPtr<FJsonObject> & t_jsonObject)
{
	TSuitPatternManager::GetInstance()->EncodeData(t_jsonObject);
}

void LoadDataModule::OnLoadPartTMsg(TSharedPtr<FJsonObject> & t_jsonObject)
{
	TSuitPartManager::GetInstance()->EncodeData(t_jsonObject);
}

void LoadDataModule::OnLoadPartComponentTMsg(TSharedPtr<FJsonObject> & t_jsonObject)
{
	TSuitComponentManager::GetInstance()->EncodeData(t_jsonObject);
}

void LoadDataModule::OnLoadStitchTMsg(TSharedPtr<FJsonObject> & t_jsonObject)
{
	TSuitStitchManager::GetInstance()->EncodeData(t_jsonObject);
}

void LoadDataModule::OnLoadAccessoriesTMsg(TSharedPtr<FJsonObject>& t_jsonObject)
{
	TSuitAccessoriesManager::GetInstance()->EncodeData(t_jsonObject);
}

void LoadDataModule::OnLoadSliceTMsg(TSharedPtr<FJsonObject> & t_jsonObject)
{
	TSuitSliceManager::GetInstance()->EncodeData(t_jsonObject);
}

void LoadDataModule::OnLoadStyleDetailRMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSharedPtr<FJsonObject> mJsonData = t_jsonObject->GetObjectField(TEXT("data"));

		// 获取R部件.
		RStyleComponentManager::GetInstance()->EncodeData(mJsonData);
		
		// 获取线迹.
		RStyleStitchManager::GetInstance()->EncodeData(mJsonData);
		
		// 获取部位.
		RStylePartManager::GetInstance()->EncodeData(mJsonData);

		// 获取部片.
		RStyleSliceManager::GetInstance()->EncodeData(mJsonData);

		// 辅料.
		RStyleAccessoriesManager::GetInstance()->EncodeData(mJsonData);

		// 获取花型T.
		TSuitPatternManager::GetInstance()->EncodePatternFromStyleDetailData(mJsonData);

		// 发送通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadStyleRDetailFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载部片成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadStyleRDetailFinish, EnumMsgResponseErrorCode::MessageFalse, TEXT("下载部片失败!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}
void LoadDataModule::OnLoadDesignTypeTMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		FString mContent;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
		UE_LOG(LogTemp, Log, TEXT("zhx : OnLoadDesignTypeTMsg : %s"), *mContent);

		TArray<TSharedPtr<FJsonValue>> mJsonArray = t_jsonObject->GetArrayField(TEXT("data"));

		TDesignTypeManager::GetInstance()->EncodeData(t_jsonObject);
		
		// 发送通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadDesignTypeFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载设计分类成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadDesignTypeFinish, EnumMsgResponseErrorCode::MessageFalse, TEXT("下载设计分类失败!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}
void LoadDataModule::OnLoadHomeInfoBikiniTMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		FString mContent;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
		UE_LOG(LogTemp, Log, TEXT("zhx : OnLoadHomeInfoTMsg : %s"), *mContent);

		HomeInfoManager::GetInstance()->EncodeData(t_jsonObject);

		// 发送通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadHomeInfoBikiniFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载设计分类成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadHomeInfoBikiniFinish, EnumMsgResponseErrorCode::MessageFalse, TEXT("下载设计分类失败!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}
void LoadDataModule::OnLoadHomeInfoSportTMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		FString mContent;
		TSharedRef<TJsonWriter<TCHAR>> t_writer = TJsonWriterFactory<>::Create(&mContent);
		FJsonSerializer::Serialize(t_jsonObject.ToSharedRef(), t_writer);
		UE_LOG(LogTemp, Log, TEXT("zhx : OnLoadHomeInfoSportTMsg : %s"), *mContent);

		HomeInfoManager::GetInstance()->EncodeSportData(t_jsonObject);

		// 发送通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadHomeInfoBikiniFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载设计分类成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadHomeInfoBikiniFinish, EnumMsgResponseErrorCode::MessageFalse, TEXT("下载设计分类失败!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

void LoadDataModule::OnLoadUserPatternMsg(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		TArray<BaseR *> mArrayRuntimeData;

		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		TSuitPatternManager::GetInstance()->EncodeData(t_jsonObject);

		// 发送通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadUserPattern, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载花型成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadUserPattern, EnumMsgResponseErrorCode::MessageFalse, TEXT("下载花型失败!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

void LoadDataModule::RequestDataT(int32 _msgId, int32 _suitId)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 请求http.
	TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

	if (_suitId != 0)
		t_jsonObject->SetNumberField(TEXT("suitId"), _suitId);

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, _msgId, mToken, mOperator, false, t_jsonObject));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}
void  LoadDataModule::LoadDesignTypeT()
{
	RequestDataT(m_loadDesignTypeFinish);
}
void LoadDataModule::LoadHomeInfoBikini()
{
	RequestDataT(m_loadHomeInfoBikiniFinish);
}
void LoadDataModule::LoadHomeInfoSport()
{
	RequestDataT(m_loadHomeInfoSportFinish);
}

void LoadDataModule::LoadUserPatterns()
{
	//获取用户花型.
	RequestDataT(m_loadUserPattern);
}

void LoadDataModule::LoadFabricLibraryT()
{
	RequestDataT(m_loadFabricLibraryFinish);
}

void LoadDataModule::LoadFabricT()
{
	RequestDataT(m_loadFabricFinish);
}

void LoadDataModule::LoadFabricColorLibraryT()
{
	//RequestDataT(m_loadFabricColorLibraryFinish);
}

void LoadDataModule::LoadFabricColorT()
{
	RequestDataT(m_loadFabricColorFinish);
}

void LoadDataModule::LoadPatternT()
{
	RequestDataT(m_loadPatternFinish);
}

void LoadDataModule::LoadPartT(int32 _suitId)
{
	if (nullptr != TSuitPartManager::GetInstance()->GetTemplateByPID(_suitId))
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadPartFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		RequestDataT(m_loadPartFinish, _suitId);
	}
}

void LoadDataModule::LoadPartComponentT(int32 _suitId)
{
	if (nullptr != TSuitComponentManager::GetInstance()->GetTemplateByPID(_suitId))
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadPartComponentFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		RequestDataT(m_loadPartComponentFinish, _suitId);
	}
}

void LoadDataModule::LoadStitchT(int32 _suitId)
{
	if (nullptr != TSuitStitchManager::GetInstance()->GetTemplateByPID(_suitId))
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadStitchFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		RequestDataT(m_loadStitchFinish, _suitId);
	}
}

void LoadDataModule::LoadSliceT(int32 _suitId)
{
	if (nullptr != TSuitSliceManager::GetInstance()->GetTemplateByPID(_suitId))
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadSliceFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		RequestDataT(m_loadSliceFinish, _suitId);
	}
}

void LoadDataModule::LoadAccessoriesT(int32 _suitId)
{
	if (nullptr != TSuitAccessoriesManager::GetInstance()->GetTemplateByPID(_suitId))
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadAccessoriesFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		RequestDataT(m_loadAccessoriesFinish, _suitId);
	}
}

void LoadDataModule::LoadStyleDetails(int32 _styleId)
{
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	StyleR * mStyleR = RSysStyleManager::GetInstance()->GetRuntimeByPID<StyleR>(_styleId);

	if (nullptr == mStyleR)
	{
		mStyleR = RUserStyleManager::GetInstance()->GetRuntimeByPID<StyleR>(_styleId);
	}

	int32 mPartId = mStyleR->m_stylePartIdArray[0];

	if (nullptr != RStylePartManager::GetInstance()->GetRuntimeByPID(mPartId))
	{
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_loadStyleRDetailFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载部片成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		// 请求http.
		TSharedPtr<FJsonObject> t_jsonObject = MakeShareable(new FJsonObject);

		t_jsonObject->SetNumberField(TEXT("styleId"), _styleId);

		// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
		FString mToken = mLoginModule->GetUserData()->GetToken();

		FString mOperator = mLoginModule->GetUserData()->GetUserId();

		msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, m_loadStyleRDetailFinish, mToken, mOperator, false, t_jsonObject));

		// 3.通过消息系统发送消息.
		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

void LoadDataModule::ClearStyleDetailData()
{
	RStyleAccessoriesManager::GetInstance()->ReleaseAllData();

	RStyleComponentManager::GetInstance()->ReleaseAllData();

	RStyleFabricColorManager::GetInstance()->ReleaseAllData();

	RStyleFabricManager::GetInstance()->ReleaseAllData();

	RStylePatternManager::GetInstance()->ReleaseAllData();

	RStylePartManager::GetInstance()->ReleaseAllData();

	RStyleSliceManager::GetInstance()->ReleaseAllData();

	RStyleStitchManager::GetInstance()->ReleaseAllData();
}

void LoadDataModule::ClearSuitDetailData()
{
	// 暂时不需要卸载这些资源.以后有必要在放开.
	/*TSuitAccessoriesManager::GetInstance()->ReleaseAllData();

	TSuitComponentManager::GetInstance()->ReleaseAllData();

	TSuitPartManager::GetInstance()->ReleaseAllData();

	TSuitSliceManager::GetInstance()->ReleaseAllData();

	TSuitStitchManager::GetInstance()->ReleaseAllData();*/
}

void LoadDataModule::DownloadDefaultData()
{
	

	// 先拿出来默认下载路径.
	m_downloadDataFinish = m_downloadDefaultDataFinish;

	TArray<XmlDownloadStruct*> mFileArray;

	GetAllDefaultDownloadFile(mFileArray);

	// 在判断md5，判断本地文件是否存在并下载.
	TArray<XmlDownloadStruct*> mOutArray;
	CheckMd5(mFileArray, mOutArray);

	// 判断哪些文件需要下载哪些文件直接读取本地.
	TArray<XmlDownloadStruct*> mLoadLocalFileArray;

	UE_LOG(LogTemp, Log, TEXT("需要热更新的xml数量****!!!**: %d"), mOutArray.Num());

	for (int32 i = 0; i < mFileArray.Num(); i++)
	{
		XmlDownloadStruct * mAllStruct = mFileArray[i];

		bool isExits = false;
		for (int32 j=0;j<mOutArray.Num();j++)
		{
			XmlDownloadStruct * mDownloadStruct = mOutArray[j];

			if (mAllStruct->m_code.Equals(mDownloadStruct->m_code) && mAllStruct->m_suitId == mDownloadStruct->m_suitId)
			{
				isExits = true;
				break;
			}
		}
		if (!isExits)
		{
			mLoadLocalFileArray.Add(mAllStruct);
		}
	}

	// 现价在本地文件到内存中.
	for (int32 i = 0; i < mLoadLocalFileArray.Num(); i++)
	{
		FString mDownloadMsg = GLanguageTool::GetInstance()->GetLanguage(TEXT("key153"));

		DownlaodInfoStruct mDownloadStruct;

		mDownloadStruct.m_DownloadMsg = mDownloadMsg;

		mDownloadStruct.m_Size = 0.0f;

		mDownloadStruct.m_Speed = 0.0f;

		mDownloadStruct.m_Progress = 1.0f;

		// 直接发送下载完成的通知.
		msg_ptr mProcessMsg(new MsgLocal(MessageTypeLocal, LogicUpdateScene::m_DownloadPak_Progress, &mDownloadStruct));

		MsgCenter::Instance()->SendMsg(mProcessMsg);

		LoadLocalJson(mLoadLocalFileArray[i]);
	}

	
	// 开始下载。下载完了发送通知.
	if (mOutArray.Num() > 0)
	{
		m_downloadTaskNum = mOutArray.Num();

		for (int32 i = 0; i < mOutArray.Num(); i++)
		{
			m_downloadQuene.Add(mOutArray[i]);

			m_allDownloadDataSize = m_allDownloadDataSize + mOutArray[i]->m_size;
		}
		StartDownload();
	}
	else
	{
		// 直接发送下载完成的通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_downloadDataFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载默认数据成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

void LoadDataModule::DownloadSuitDetailData(int32 _suitId)
{
	m_downloadDataFinish = m_downloadSuitDetailFinish;

	// 判断内存中有数据.就不加载磁盘.
	SuitT * mSuitT = TSuitManager::GetInstance()->GetTemplateByPID<SuitT>(_suitId);

	if (nullptr != mSuitT && mSuitT->m_ArrayPartId.Num()>0)
	{
		BaseT * mBaseT = TSuitPartManager::GetInstance()->GetTemplateByPID(mSuitT->m_ArrayPartId[0]);

		if (nullptr != mBaseT)
		{
			msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_downloadDataFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载默认数据成功!")));

			MsgCenter::Instance()->SendMsg(mMsg);
			return;
		}
	}
	// 先拿出来默认下载路径.
	TArray<XmlDownloadStruct*> mFileArray;

	GetSuitDetailDownloadFile(mFileArray, _suitId);

	// 在判断md5，判断本地文件是否存在并下载.
	TArray<XmlDownloadStruct*> mOutArray;
	CheckMd5(mFileArray, mOutArray);

	// 判断哪些文件需要下载哪些文件直接读取本地.
	TArray<XmlDownloadStruct*> mLoadLocalFileArray;

	for (int32 i = 0; i < mFileArray.Num(); i++)
	{
		XmlDownloadStruct * mAllStruct = mFileArray[i];

		bool isExits = false;
		for (int32 j = 0; j < mOutArray.Num(); j++)
		{
			XmlDownloadStruct * mDownloadStruct = mOutArray[j];

			if (mAllStruct->m_code.Equals(mDownloadStruct->m_code)&& mAllStruct->m_suitId == mDownloadStruct->m_suitId)
			{
				isExits = true;
				break;
			}
		}
		if (!isExits)
		{
			mLoadLocalFileArray.Add(mAllStruct);
		}
	}

	// 现价在本地文件到内存中.
	for (int32 i = 0; i < mLoadLocalFileArray.Num(); i++)
	{
		LoadLocalJson(mLoadLocalFileArray[i]);
	}
	m_downloadTaskNum = mOutArray.Num();
	// 开始下载。下载完了发送通知.
	if (mOutArray.Num() > 0)
	{
		for (int32 i = 0; i < mOutArray.Num(); i++)
		{
			m_downloadQuene.Add(mOutArray[i]);
		}
		StartDownload();
	}
	else
	{
		// 直接发送下载完成的通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_downloadDataFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载默认数据成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

void LoadDataModule::InitDownloadData()
{
	OnLoadLocalXml();

	// 1.获取下载xml的url.
	LoginModule * mLoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");

	// 2.封装消息体 channelId,消息号，token，_operator，是否Get或Post,发送的消息内容.
	FString mToken = mLoginModule->GetUserData()->GetToken();

	FString mOperator = mLoginModule->GetUserData()->GetUserId();

	msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4018, mToken, mOperator, false));

	// 3.通过消息系统发送消息.
	MsgCenter::Instance()->SendMsg(mMsg);
}

void LoadDataModule::OnUpdate(float DeltaTime)
{

}

void LoadDataModule::OnLoadLocalXml()
{
	FString mLocalXmlContent;

	bool isLoadSucess = GFileHelper::LoadFileToString(mLocalXmlContent,*m_localSaveXmlPath);

	if (isLoadSucess)
	{
		TiXmlDocument doc;

		std::string stdContent = TCHAR_TO_UTF8(*mLocalXmlContent);

		const char * xmlContent = stdContent.c_str();

		doc.Parse(xmlContent);

		TiXmlElement* root = doc.FirstChildElement();

		if (root == NULL)
		{
			doc.Clear();

			return;
		}

		TiXmlElement* data = root->FirstChildElement();

		for (TiXmlElement* elem = data->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			const char * mCode = elem->Attribute("code");
			const char * mIsDownload = elem->Attribute("isDownload");
			const char * mMd5 = elem->Attribute("md5");
			const char * mSize = elem->Attribute("size");
			const char * mSuitId = elem->Attribute("suitId");
			const char * mUrl = elem->Attribute("url");

			XmlDownloadStruct * mStruct = new XmlDownloadStruct();
			mStruct->m_code = UTF8_TO_TCHAR(mCode);
			mStruct->m_isDownload = atoi(mIsDownload);
			mStruct->m_size = atoi(mSize);
			mStruct->m_md5 = UTF8_TO_TCHAR(mMd5);
			mStruct->m_suitId = atoi(mSuitId);
			mStruct->m_url = UTF8_TO_TCHAR(mUrl);

			m_localXmlDownloadStructArray.Add(mStruct);
		}

		doc.Clear();
	}
}

void LoadDataModule::OnGetXmlUrl(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		TSharedPtr<FJsonObject> t_jsonObject = mHttpMsg->GetHttpContent();

		FString mXmlUrl;

		t_jsonObject->TryGetStringField(TEXT("data"),mXmlUrl);

		// 直接下载xml内容.
		msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4019, mXmlUrl,nullptr,true));

		// 3.通过消息系统发送消息.
		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		// 下载失败.
	}
}

void LoadDataModule::OnGetXmlData(msg_ptr _msg)
{
	MsgHttp *mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		//Byte *  mByteMsg = mHttpMsg->GetByteContent();

		//int32 mByteSize = mHttpMsg->GetByteLenght();

		//// 转换成字符串.
		//if (mByteSize > 0)
		//{
			FString str = mHttpMsg->m_httpContent;

			UE_LOG(LogTemp, Log, TEXT("zhx : OnLoadHomeInfoTMsg : %s"), *str);

			OnXmlParser(str);
		//}
	}
	else
	{
		// 下载失败.
	}
}

void LoadDataModule::OnXmlParser(FString & _xmlContent)
{
	TiXmlDocument doc;

	std::string stdContent = TCHAR_TO_UTF8(*_xmlContent);

	const char * xmlContent = stdContent.c_str();

	doc.Parse(xmlContent);

	TiXmlElement* root = doc.FirstChildElement();

	if (root == NULL)
	{
		doc.Clear();

		return;
	}

	TiXmlElement* data = root->FirstChildElement();

	for (TiXmlElement* elem = data->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mCode = elem->Attribute("code");
		const char * mIsDownload = elem->Attribute("isDownload");
		const char * mMd5 = elem->Attribute("md5");
		const char * mSize = elem->Attribute("size");
		const char * mSuitId = elem->Attribute("suitId");
		const char * mUrl = elem->Attribute("url");

		XmlDownloadStruct * mStruct = new XmlDownloadStruct();
		mStruct->m_code = UTF8_TO_TCHAR(mCode);
		mStruct->m_isDownload = atoi(mIsDownload);
		mStruct->m_size = atoi(mSize);
		mStruct->m_md5 = UTF8_TO_TCHAR(mMd5);
		mStruct->m_suitId = atoi(mSuitId);
		mStruct->m_url = UTF8_TO_TCHAR(mUrl);

		m_serverXmlDownloadStructArray.Add(mStruct);
	}

	doc.Clear();

	// 发送通知.
	msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_initXmlFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("初始化xml成功!")));

	MsgCenter::Instance()->SendMsg(mMsg);
}

void LoadDataModule::SaveLocalXml()
{
	// 把xml转json并保存到本地.
	FString mSaveXmlContent;

	TiXmlDocument * doc = new TiXmlDocument();

	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(pDeclaration);

	TiXmlElement * mElement = new TiXmlElement("ROOT");

	doc->LinkEndChild(mElement);

	TiXmlElement * mData = new TiXmlElement("DATA");

	mElement->LinkEndChild(mData);

	for (int32 i=0;i<m_localXmlDownloadStructArray.Num();i++)
	{
		XmlDownloadStruct * mStruct = m_localXmlDownloadStructArray[i];

		TiXmlElement * mFile = new TiXmlElement("FILE");

		mFile->SetAttribute("code", TCHAR_TO_UTF8(*mStruct->m_code));

		mFile->SetAttribute("isDownload", mStruct->m_isDownload);

		mFile->SetAttribute("md5", TCHAR_TO_UTF8(*mStruct->m_md5));

		mFile->SetAttribute("size", mStruct->m_size);

		mFile->SetAttribute("suitId", mStruct->m_suitId);

		mFile->SetAttribute("url", TCHAR_TO_UTF8(*mStruct->m_url));

		mData->LinkEndChild(mFile);
	}

	// 保存到指定的磁盘中.
	doc->SaveFile(TCHAR_TO_UTF8(*m_localSaveXmlPath));

	//GFileHelper::SaveStringToFile(UTF8_TO_TCHAR(mStr),*m_localSaveXmlPath);

	doc->Clear();

	delete doc;
}

void LoadDataModule::GetAllDefaultDownloadFile(TArray<XmlDownloadStruct *> & _downloadFileArray)
{
	for (int32 i = 0; i < m_serverXmlDownloadStructArray.Num(); i++)
	{
		XmlDownloadStruct * mStruct = m_serverXmlDownloadStructArray[i];
		// 过滤掉。。以后不需要下载系统花型了.
		if (mStruct->m_isDownload == 1 && !mStruct->m_code.Equals(TEXT("pattern")))
		{
			_downloadFileArray.Add(mStruct);
		}
	}
}

FString LoadDataModule::GetLocalFile(XmlDownloadStruct * _serverPath)
{
	// 拼接路径.
	FString mPresentPath = GFileHelper::ProjectPersistentDownloadDir();

	FString mSavePath = FString::Printf(TEXT("%s/Download/Xml/%s_%d.json"), *mPresentPath, *_serverPath->m_code, _serverPath->m_suitId);

	GPlatformFileManager::RecursiveCreateDirectory(*mSavePath);

	return mSavePath;
}

bool LoadDataModule::CheckFileExits(const FString & _filePath)
{
	return GPlatformFileManager::FileExists(*_filePath);
}

void LoadDataModule::CheckMd5(TArray<XmlDownloadStruct *> & _downloadArray, TArray<XmlDownloadStruct *> & _outArray)
{
	for (int32 i=0;i<_downloadArray.Num();i++)
	{
		XmlDownloadStruct * mStruct = _downloadArray[i];

		bool isDownload = false;

		if (!CheckFileExits(GetLocalFile(mStruct)))
		{
			isDownload = true;
		}
		else
		{
			if (m_localXmlDownloadStructArray.Num() > 0)
			{
				for (int32 j = 0; j < m_localXmlDownloadStructArray.Num(); j++)
				{
					XmlDownloadStruct * mLocalStruct = m_localXmlDownloadStructArray[j];

					// 判断MD5.
					if (mLocalStruct->m_code.Equals(mStruct->m_code) && mLocalStruct->m_suitId == mStruct->m_suitId)
					{
						if (!mLocalStruct->m_md5.Equals(mStruct->m_md5))
							isDownload = true;
						break;
					}
				}
			}
			else
			{
				isDownload = true;
			}
		}
		if (isDownload)
			_outArray.Add(mStruct);
	}
}

void LoadDataModule::StartDownload()
{
	DownlaodInfoStruct mDownloadStruct;

	FString mDownloadMsg = GLanguageTool::GetInstance()->GetLanguage(TEXT("key152"));

	mDownloadStruct.m_DownloadMsg = mDownloadMsg;

	mDownloadStruct.m_Size = (float)(float(m_allDownloadDataSize) / 1024.0f/1024.0f);

	mDownloadStruct.m_Speed = 0.0f;

	mDownloadStruct.m_Progress = float(m_downloadTaskNum - m_downloadQuene.Num()) / (float)m_downloadTaskNum;

	// 直接发送下载完成的通知.
	msg_ptr mProcessMsg(new MsgLocal(MessageTypeLocal, LogicUpdateScene::m_DownloadPak_Progress, &mDownloadStruct));

	MsgCenter::Instance()->SendMsg(mProcessMsg);

	if (m_downloadQuene.Num()>0)
	{
		XmlDownloadStruct * mLocalStruct = m_downloadQuene[0];

		m_downloadQuene.RemoveAt(0);

		// 直接下载xml内容.
		msg_ptr mMsg(new MsgHttp(MessageTypeHttpRequest, 4021, mLocalStruct->m_url, mLocalStruct, true));

		// 3.通过消息系统发送消息.
		MsgCenter::Instance()->SendMsg(mMsg);
	}
	else
	{
		// 直接发送下载完成的通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_downloadDataFinish, EnumMsgResponseErrorCode::MessageTrue, TEXT("加载默认数据成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

void LoadDataModule::OnGetXmlContent(msg_ptr _msg)
{
	// 解析xml，并加载到内存当中.继续掉用递归下载别的数据.
	MsgHttp * mHttpMsg = _msg->ConvertMsg<MsgHttp>();

	// 透传的参数.
	XmlDownloadStruct * mServerStruct = (XmlDownloadStruct *)mHttpMsg->m_httpPenetrate;

	// 1.先判断消息是否正常接收.
	if (mHttpMsg->GetMsgErrorCode() == EnumMsgResponseErrorCode::MessageTrue)
	{
		// 2.消息正常,获取内容.
		//Byte *  mByteMsg = mHttpMsg->GetByteContent();

		//int32 mByteSize = mHttpMsg->GetByteLenght();

		//// 转换成字符串.
		//if (mByteSize > 0)
		{
			/*const std::string cstr(reinterpret_cast<const char*>(mByteMsg), mByteSize);

			FString mResponseMsg(UTF8_TO_TCHAR(cstr.c_str()));*/
			FString mResponseMsg = mHttpMsg->m_httpContent;

			// 下载的内容是json,反序列化并保存到本地和内存当中！..
			TSharedPtr<FJsonObject> mJsonObject;

			TSharedRef<TJsonReader<>> t_reader = TJsonReaderFactory<>::Create(mResponseMsg);
			if (FJsonSerializer::Deserialize(t_reader, mJsonObject))
			{
				// 根据code判断到底加载哪些数据.
				LoadJsonByCode(mJsonObject, mServerStruct->m_code);
			}
			FString mPresentPath = GFileHelper::ProjectPersistentDownloadDir();

			FString mSavePath = FString::Printf(TEXT("%s/Download/Xml/%s_%d.json"), *mPresentPath, *mServerStruct->m_code, mServerStruct->m_suitId);

			SaveDownloadJson(mResponseMsg, mSavePath);

			// 刷新一下本地json md5并保存.
			bool isExists = false;
			for (int32 i=0;i<m_localXmlDownloadStructArray.Num();i++)
			{
				XmlDownloadStruct* mLocalStruct = m_localXmlDownloadStructArray[i];

				if (mLocalStruct->m_code.Equals(mServerStruct->m_code)&& mLocalStruct->m_suitId == mServerStruct->m_suitId)
				{
					isExists = true;
					mLocalStruct->m_url = mServerStruct->m_url;
					mLocalStruct->m_md5 = mServerStruct->m_md5;
				}
			}
			if (!isExists)
			{
				XmlDownloadStruct* mTempStruct = new XmlDownloadStruct();

				mTempStruct->m_code = mServerStruct->m_code;
				
				mTempStruct->m_isDownload = mServerStruct->m_isDownload;

				mTempStruct->m_md5 = mServerStruct->m_md5;

				mTempStruct->m_size = mServerStruct->m_size;

				mTempStruct->m_suitId = mServerStruct->m_suitId;

				mTempStruct->m_url = mServerStruct->m_url;

				m_localXmlDownloadStructArray.Add(mTempStruct);
			}
			SaveLocalXml();

			StartDownload();
		}
	}
	else
	{
		// 下载失败.
		// 直接发送下载完成的通知.
		msg_ptr mMsg(new MsgLocal(MessageTypeLocal, m_downloadDataFinish, EnumMsgResponseErrorCode::MessageFalse, TEXT("加载默认数据成功!")));

		MsgCenter::Instance()->SendMsg(mMsg);
	}
}

void LoadDataModule::SaveDownloadJson(const FString & _content, const FString & _path)
{
	GFileHelper::SaveStringToFile(_content, *_path);
}

void LoadDataModule::LoadLocalJson(XmlDownloadStruct * _localJson)
{
	FString mPresentPath = GFileHelper::ProjectPersistentDownloadDir();

	FString mSavePath = FString::Printf(TEXT("%s/Download/Xml/%s_%d.json"), *mPresentPath, *_localJson->m_code, _localJson->m_suitId);

	GPlatformFileManager::RecursiveCreateDirectory(*mSavePath);

	FString mJsonContent;
	GFileHelper::LoadFileToString(mJsonContent,*mSavePath);

	TSharedPtr<FJsonObject> mJsonObject;

	TSharedRef<TJsonReader<>> t_reader = TJsonReaderFactory<>::Create(mJsonContent);
	if (FJsonSerializer::Deserialize(t_reader, mJsonObject))
	{
		// 根据code判断到底加载哪些数据.
		LoadJsonByCode(mJsonObject, _localJson->m_code);
	}
}

void LoadDataModule::LoadJsonByCode(TSharedPtr<FJsonObject> & mJsonObject, const FString & _jsonCode)
{
	if (_jsonCode.Equals(TEXT("suits")))
	{
		// 加载套件.
		OnLoadSuitTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("suitFabric")))
	{
		// 加载套件.
		OnLoadFabricTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("suitFabricLibrary")))
	{
		// 加载套件.
		OnLoadFabricLibraryTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("suitFabricColor")))
	{
		// 加载套件.
		OnLoadFabricColorTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("part_clothes")))
	{
		// 加载套件.
		OnLoadPartTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("part_accessories"))) 
	{
		// 加载套件.
		OnLoadAccessoriesTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("part_line")))
	{
		// 加载套件.
		OnLoadStitchTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("component")))
	{
		// 加载套件.
		OnLoadPartComponentTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("selice")))
	{
		// 加载套件.
		OnLoadSliceTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("selice")))
	{
		// 加载套件.
		OnLoadSliceTMsg(mJsonObject);
	}
	else if (_jsonCode.Equals(TEXT("pattern")))
	{
		// 加载套件.
		OnLoadPatternTMsg(mJsonObject);
	}
}

void LoadDataModule::GetSuitDetailDownloadFile(TArray<XmlDownloadStruct *> & _downloadFileArray, int32 _suitId)
{
	// 获取部位。部件，线迹，辅料，部片.
	for (int32 i=0;i<m_serverXmlDownloadStructArray.Num();i++)
	{
		XmlDownloadStruct * mServerStruct = m_serverXmlDownloadStructArray[i];

		if (mServerStruct->m_suitId == _suitId)
		{
			if (mServerStruct->m_code.Equals(TEXT("part_clothes")) ||
				mServerStruct->m_code.Equals(TEXT("component")) ||
				mServerStruct->m_code.Equals(TEXT("part_line")) ||
				mServerStruct->m_code.Equals(TEXT("part_accessories")) ||
				mServerStruct->m_code.Equals(TEXT("selice")))
			{
				_downloadFileArray.Add(mServerStruct);
			}
		}
	}
}
