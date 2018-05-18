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
	
	// 加载系统款数据异常的回调.
	void LoadSysStyleError(FString _errorInfo);

	// 加载系列完毕的回调.
	void LoadSysStyleFinishById(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData);

	// 加载系统款完毕的回调.
	void LoadSysStyleFinish(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData);

	// 加载用户款数据异常的回调.
	void LoadUserStyleError(FString _errorInfo);

	// 加载用户款完毕的回调.
	void LoadUserStyleFinish(int32 _pageIndex, int32 _pageSize, int32 _recordCount, int32 _totalPage, TArray<BaseR*> & _ArrayRuntimData);

	void LoadMyWorksContentStyleData(int32 _num);
	void LoadMyWorksContentStyleError(FString _errorInfo);
	void LoadMyWorksContentStyleFinish(msg_ptr _msg);
	
	TArray<SelectCellData> GetSelectCellDatas();

	TArray<SelectCellData> GetMyWorksSelectCellDatas();	//	获得所有作品的Json数据.

	TArray<SelectCellData> GetSysSelectCellDatas_ToCreate();

	TArray<int32> GetSuitSubTypes();

	FString GetStyleSubTypeName(int32 catid);

	void AddSuitSubType(int32 inCatid, const FString & inName);
	
	
public:
	TMap<int32, FString> CacheSuitSubTypes;

	TArray<int32> CacheSortSuitSubTypes;

	TArray<SelectCellData> m_MainUMG_SelectCell_Datas;

	TArray<SelectCellData> m_MyWorks_SelectCell_Dats;	//	存储所有作品json.

	TArray<SelectCellData> m_Sys_SelectCell_Dats;

	TArray<SelectCellData> m_Sys_SelectCell_Dats_ById;

	int32 m_MyWorks_Content_LocalPid;

	int32 m_AllWorks_Content_LocalPid;	//	我的作品本地消息号.

	int32 m_PersonInfo_Back = 2017;		//	个人中心返回.

};