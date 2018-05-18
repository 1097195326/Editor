#pragma once
#include "CoreMinimal.h"
#include "MsgCenter.h"
#include "UserData.h"
#include "LoadDataModule.h"
#include "SelectCell.h"
#include "System/BaseData/BaseModule.h"

/**
*.
*/
class ResHttp
{
public:
	ResHttp():m_pageIndex(0), m_pageSize(0), m_recordCount(0), m_totalPage(0){}
	int32 m_pageIndex;
	int32 m_pageSize;
	int32 m_recordCount;
	int32 m_totalPage;
};

class EDITOR_API MainModule : public BaseModule
{
public:

	MainModule();

	virtual ~MainModule();

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	virtual void GetSystemStyleData(int32 _pageIndex, int32 _pageSize, TArray<int32> _catIds = {});

	void GetSystemStyleDataByID(int32 _id);

	virtual void GetUserStyleData(int32 _pageIndex,int32 _pageSize);
	
	// ����ϵͳ�������쳣�Ļص�.
	void LoadSysStyleError(FString _errorInfo);

	// ����ϵ����ϵĻص�.
	void LoadSysStyleFinishById(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData);

	// ����ϵͳ����ϵĻص�.
	void LoadSysStyleFinish(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData);

	// �����û��������쳣�Ļص�.
	void LoadUserStyleError(FString _errorInfo);

	// �����û�����ϵĻص�.
	void LoadUserStyleFinish(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData);

	void LoadMyWorksContentStyleData(int32 _num);
	void LoadMyWorksContentStyleError(FString _errorInfo);
	void LoadMyWorksContentStyleFinish(msg_ptr _msg);
	
	TArray<SelectCellData> GetSelectCellDatas();

	TArray<SelectCellData> GetMyWorksSelectCellDatas();	//	���������Ʒ��Json����.

	TArray<SelectCellData> GetSysSelectCellDatas_ToCreate();

	TArray<int32> GetSuitSubTypes();

	FString GetStyleSubTypeName(int32 catid);

	void AddSuitSubType(int32 inCatid, const FString & inName);
	
	
public:
	TMap<int32, FString> CacheSuitSubTypes;

	TArray<int32> CacheSortSuitSubTypes;

	TArray<SelectCellData> m_MainUMG_SelectCell_Datas;

	TArray<SelectCellData> m_MyWorks_SelectCell_Dats;	//	�洢������Ʒjson.

	TArray<SelectCellData> m_Sys_SelectCell_Dats;

	TArray<SelectCellData> m_Sys_SelectCell_Dats_ById;

	int32 m_MyWorks_Content_LocalPid;

	int32 m_AllWorks_Content_LocalPid;	//	�ҵ���Ʒ������Ϣ��.

	int32 m_PersonInfo_Back = 2017;		//	�������ķ���.

};