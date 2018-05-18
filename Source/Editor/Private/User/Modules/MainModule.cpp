#include "MainModule.h"
#include "StyleR.h"
#include "Editor.h"

GREGISTER_MODULE(MainModule)

MainModule::MainModule()
{
	m_MyWorks_Content_LocalPid = 3009;

	m_AllWorks_Content_LocalPid = 201;
}

MainModule::~MainModule()
{

}

void MainModule::OnLoad()
{
	AddSuitSubType(1007, TEXT("连体泳裙"));
	AddSuitSubType(1008, TEXT("连体泳裤"));
	AddSuitSubType(1009, TEXT("比基尼"));
	MsgCenter::Instance()->RegisterMsgHandler(MessageTypeLocal, 4102, this, &MainModule::LoadMyWorksContentStyleFinish);
}
void MainModule::AddSuitSubType(int32 inCatid, const FString & inName)
{
	CacheSuitSubTypes.Add(inCatid, inName);
	CacheSortSuitSubTypes.Add(inCatid);
}


void MainModule::OnRelease()
{
	MsgCenter::Instance()->RemoveMsgHandler(MessageTypeLocal, 4102, this, &MainModule::LoadMyWorksContentStyleFinish);
}

void MainModule::GetSystemStyleData(int32 _pageIndex, int32 _pageSize, TArray<int32> _catIds)
{
	RSysStyleManager::GetInstance()->BindLoadErrorDelegate(this,&MainModule::LoadSysStyleError);

	RSysStyleManager::GetInstance()->BindLoadFinishDelegate(this,&MainModule::LoadSysStyleFinish);

	RSysStyleManager::GetInstance()->LoadStyle(_pageIndex,_pageSize, _catIds);
}
void MainModule::GetSystemStyleDataByID(int32 _id)
{
	RSysStyleManager::GetInstance()->BindLoadErrorDelegate(this, &MainModule::LoadSysStyleError);

	RSysStyleManager::GetInstance()->BindLoadFinishDelegate(this, &MainModule::LoadSysStyleFinishById);

	RSysStyleManager::GetInstance()->LoadStyleById(_id);
}

void MainModule::GetUserStyleData(int32 _pageIndex, int32 _pageSize)
{
	RUserStyleManager::GetInstance()->BindLoadErrorDelegate(this, &MainModule::LoadUserStyleError);

	RUserStyleManager::GetInstance()->BindLoadFinishDelegate(this, &MainModule::LoadUserStyleFinish);

	RUserStyleManager::GetInstance()->LoadStyle(_pageIndex, _pageSize);
}

void MainModule::LoadMyWorksContentStyleData(int32 _num)
{
	// 监听4102本地消息.

	RUserStyleManager::GetInstance()->LoadStyleForMainUI(1, _num);
}

void MainModule::LoadMyWorksContentStyleError(FString _errorInfo)
{
	UE_LOG(GEditorLog, Error, TEXT("MyWorksContentStyle Icon url :%s"), *_errorInfo);
}

void MainModule::LoadMyWorksContentStyleFinish(msg_ptr _msg)
{
	MsgLocal * mMsg = _msg->ConvertMsg<MsgLocal>();
	LoadUserStyleData * mUserStyleData = (LoadUserStyleData *)mMsg->Content();

	if (!mUserStyleData->_msgSuccess)
	{
		//LoadMyWorksContentStyleData();
		return;
	}

	m_MainUMG_SelectCell_Datas.Empty();
	for (size_t i = 0; i < mUserStyleData->_ArrayRuntimData.Num(); ++i)
	{
		StyleR * mStyleR = (StyleR*)mUserStyleData->_ArrayRuntimData[i];
		SelectCellData mSelectCellData;
		mSelectCellData.m_IconUrl = mStyleR->m_iconUrl;
		mSelectCellData.m_Rid = mStyleR->GetRid();
		mSelectCellData.m_Tid = mStyleR->GetTid();
		m_MainUMG_SelectCell_Datas.Add(mSelectCellData);
	}
	if (mUserStyleData->_ArrayRuntimData.Num()>0)
	{
		msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, m_MyWorks_Content_LocalPid, nullptr));
		MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);
	}
}

TArray<SelectCellData> MainModule::GetSelectCellDatas()
{
	return m_MainUMG_SelectCell_Datas;
}

TArray<SelectCellData> MainModule::GetMyWorksSelectCellDatas()
{
	return m_MyWorks_SelectCell_Dats;
}
TArray<SelectCellData> MainModule::GetSysSelectCellDatas_ToCreate()
{
	return m_Sys_SelectCell_Dats;
}

void MainModule::LoadSysStyleError(FString _errorInfo)
{
	//.
	UE_LOG(GEditorLog, Error, TEXT("style Icon url :%s"), *_errorInfo);
}

void MainModule::LoadSysStyleFinishById(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData)
{
	UE_LOG(GEditorLog, Error, TEXT("ALoginGameMode::LoadFinish %d,%d,%d,%d"), _pageIndex, _pageSize, _recordCount, _totalPage);
	m_Sys_SelectCell_Dats_ById.Empty();

	for (size_t i = 0; i < _ArrayRuntimData.Num(); ++i)
	{
		StyleR * mStyleR = (StyleR*)_ArrayRuntimData[i];
		SelectCellData mSelectCellData;
		mSelectCellData.m_IconUrl = mStyleR->m_iconUrl;
		mSelectCellData.m_Rid = mStyleR->GetRid();
		mSelectCellData.m_Tid = mStyleR->GetTid();
		m_Sys_SelectCell_Dats_ById.Add(mSelectCellData);

	}
	if (_ArrayRuntimData.Num() > 0)
	{
		msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, 1022, nullptr));
		MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);
	}
}
void MainModule::LoadSysStyleFinish(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData)
{
	UE_LOG(GEditorLog, Error, TEXT("ALoginGameMode::LoadFinish %d,%d,%d,%d"), _pageIndex, _pageSize, _recordCount, _totalPage);
	ResHttp * rh = new ResHttp();
	rh->m_pageIndex = _pageIndex;
	rh->m_pageSize = _pageSize;
	rh->m_recordCount = _recordCount;
	rh->m_totalPage = _totalPage;

	m_Sys_SelectCell_Dats.Empty();

	for (size_t i = 0; i < _ArrayRuntimData.Num(); ++i)
	{
		StyleR * mStyleR = (StyleR*)_ArrayRuntimData[i];
		SelectCellData mSelectCellData;
		mSelectCellData.m_IconUrl = mStyleR->m_iconUrl;
		mSelectCellData.m_Rid = mStyleR->GetRid();
		mSelectCellData.m_Tid = mStyleR->GetTid();
		m_Sys_SelectCell_Dats.Add(mSelectCellData);

	}
	if (_ArrayRuntimData.Num() > 0)
	{
		msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, 109, rh));
		MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);
	}
}

void MainModule::LoadUserStyleError(FString _errorInfo)
{
	//.
	UE_LOG(GEditorLog, Error, TEXT("style Icon url :%s"), *_errorInfo);
}
void MainModule::LoadUserStyleFinish(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData)
{
	m_MyWorks_SelectCell_Dats.Empty();

	for (int32 i = 0; i < _ArrayRuntimData.Num(); ++i)
	{
		StyleR * mStyleR = (StyleR*)_ArrayRuntimData[i];

		SelectCellData mSelectCellData;

		mSelectCellData.m_IconUrl = mStyleR->m_iconUrl;

		mSelectCellData.m_Rid = mStyleR->GetRid();

		mSelectCellData.m_Tid = mStyleR->GetTid();

		m_MyWorks_SelectCell_Dats.Add(mSelectCellData);
	}
	UE_LOG(LogTemp, Log, TEXT("zhx : LoadUserStyleFinish index : %d, size : %d, cont : %d, totalPage : %d"), _pageIndex, _pageSize, _recordCount, _totalPage);

	ResHttp rh;
	rh.m_pageIndex = _pageIndex;
	rh.m_pageSize = _pageSize;
	rh.m_recordCount = _recordCount;
	rh.m_totalPage = _totalPage;

	if (_ArrayRuntimData.Num() > 0)
	{
		msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, m_AllWorks_Content_LocalPid, &rh));

		MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);
	}
}

TArray<int32> MainModule::GetSuitSubTypes()
{
	return CacheSortSuitSubTypes;
}

FString MainModule::GetStyleSubTypeName(int32 catid)
{
	if (CacheSuitSubTypes.Contains(catid)) {
		return CacheSuitSubTypes[catid];
	}
	return FString();
}
