// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MsgCenter.h"
#include "RSysStyleManager.h"
#include "RUserStyleManager.h"
#include "IUpdate.h"
#include "Editor.h"
#include "System/BaseData/BaseModule.h"

struct XmlDownloadStruct
{
	FString m_code;

	int32 m_isDownload = 0;

	FString m_md5;

	int32 m_size = 0;

	int32 m_suitId = 0;

	FString m_url;
};

/*
 *
 * 数据加载内存过程.
 */
class EDITOR_API LoadDataModule : public BaseModule,public IUpdate
{
private:
	TArray<XmlDownloadStruct*> m_localXmlDownloadStructArray;

	TArray<XmlDownloadStruct*> m_serverXmlDownloadStructArray;

	FString m_localSaveXmlPath;

	// 下载xml的队列.
	TArray<XmlDownloadStruct*> m_downloadQuene;

	// 当前下载数据完成的回调.
	int32 m_downloadDataFinish = 0;

public:// 本地消息广播号.

	// 下载数据完成的回调.
	const static int32 m_downloadDefaultDataFinish = 4020;

	const static int32 m_downloadSuitDetailFinish = 4023;

	// 初始化xml完成的回调.
	const static int32 m_initXmlFinish = 4022;

	
	// 加载完成套件T消息.
	const static int32 m_loadSuitFinish = 4003;

	// 加载完成部位T消息.
	const static int32 m_loadPartFinish = 4004;

	// 加载完成部件T消息.
	const static int32 m_loadPartComponentFinish = 4005;

	// 加载完成面料库T消息.
	const static int32 m_loadFabricLibraryFinish = 4006;

	// 加载完成面料T消息.
	const static int32 m_loadFabricFinish = 4007;

	// 加载完成花色库T消息.
	const static int32 m_loadFabricColorLibraryFinish = 4008;

	// 加载完成花色T消息.
	const static int32 m_loadFabricColorFinish = 4009;

	// 加载完成花型T消息.
	const static int32 m_loadPatternFinish = 4010;

	// 加载完成部片T消息.
	const static int32 m_loadSliceFinish = 4011;

	// 加载完成线迹T消息.
	const static int32 m_loadStitchFinish = 4012;

	// 加载完成辅料T消息.
	const static int32 m_loadAccessoriesFinish = 4013;

	// 加载完成款详情的R消息.
	const static int32 m_loadStyleRDetailFinish = 4014;

	//软件设计分类数据
	const static int32 m_loadDesignTypeFinish = 1020;
	
	//CMS首页信息数据
	const static int32 m_loadHomeInfoBikiniFinish = 1021;

	const static int32 m_loadHomeInfoSportFinish = 1023;

	// 加载用户花型.
	const static int32 m_loadUserPattern = 4017;

private:
	
	int32 m_downloadTaskNum = 0;

	int32 m_allDownloadDataSize = 0;

public:

	LoadDataModule();

	virtual ~LoadDataModule();

	virtual void OnLoad() override;

	virtual void OnRelease() override;


private:// 收到服务器消息的代理区.

	// 收到套件SuitT消息.
	void OnLoadSuitTMsg(TSharedPtr<FJsonObject> & mJsonObject);

	// 面料库T收到的消息.
	void OnLoadFabricLibraryTMsg(TSharedPtr<FJsonObject> & mJsonObject);

	// 面料T收到的消息.
	void OnLoadFabricTMsg(TSharedPtr<FJsonObject> & t_jsonObject);

	// 花色T收到的消息.
	void OnLoadFabricColorTMsg(TSharedPtr<FJsonObject> & t_jsonObject);

	// 花型T收到的消息.
	void OnLoadPatternTMsg(TSharedPtr<FJsonObject> & t_jsonObject);

	// 部位T收到的消息.
	void OnLoadPartTMsg(TSharedPtr<FJsonObject> & t_jsonObject);

	// 部片T收到的消息.
	void OnLoadPartComponentTMsg(TSharedPtr<FJsonObject> & t_jsonObject);

	// 线迹T收到的消息.
	void OnLoadStitchTMsg(TSharedPtr<FJsonObject> & t_jsonObject);

	// 辅料T收到的消息.
	void OnLoadAccessoriesTMsg(TSharedPtr<FJsonObject>& t_jsonObject);

	// 部片T收到的消息.
	void OnLoadSliceTMsg(TSharedPtr<FJsonObject> & t_jsonObject);

	// 款详情收到的消息.
	void OnLoadStyleDetailRMsg(msg_ptr _msg);

	void RequestDataT(int32 _msgId, int32 _suitId=0);

	// 软件设计分类数据
	void OnLoadDesignTypeTMsg(msg_ptr _msg);

	// CMS首页信息数据
	void OnLoadHomeInfoBikiniTMsg(msg_ptr _msg);

	void OnLoadHomeInfoSportTMsg(msg_ptr _msg);

	// 收到用户花型信息.
	void OnLoadUserPatternMsg(msg_ptr _msg);


public: // 加载数据的入口方法区.
	// CMS首页信息数据
	void LoadHomeInfoBikini();
	void LoadHomeInfoSport();
	// 软件设计分类数据
	void LoadDesignTypeT();

	// 获取用户花型.
	void LoadUserPatterns();

	// 加载面料库T.
	void LoadFabricLibraryT();

	// 加载面料T.
	void LoadFabricT();

	// 加载花色库T.
	void LoadFabricColorLibraryT();

	// 加载花色T.
	void LoadFabricColorT();

	// 加载花型.
	void LoadPatternT();

	// 加载某套件下的部位T.
	void LoadPartT(int32 _suitId);

	// 加载某套件下的部件T.
	void LoadPartComponentT(int32 _suitId);

	// 加载某套件下的线迹T.
	void LoadStitchT(int32 _suitId);

	// 加载某套件下的部片T.
	void LoadSliceT(int32 _suitId);

	// 加载某套件下的辅料T.
	void LoadAccessoriesT(int32 _suitId);

	// 加载styleR的详情.
	void LoadStyleDetails(int32 _styleId);

public:

	// 清空R 详情数据.
	void ClearStyleDetailData();

	// 清空T 详情数据.
	void ClearSuitDetailData();

	// 下载默认数据.
	void DownloadDefaultData();

	// 下载套件详情数据.
	void DownloadSuitDetailData(int32 _suitId);

	// 初始化Init.
	void InitDownloadData();

	// 检查有没有任务.
	virtual void OnUpdate(float DeltaTime) override;

private:
	// 读取本地xml.
	void OnLoadLocalXml();
	
	void OnGetXmlUrl(msg_ptr _msg);

	void OnGetXmlData(msg_ptr _msg);

	void OnXmlParser(FString & _xmlContent);

	// 把最新的xml保存在本地.
	void SaveLocalXml();

	// 获取默认下载的路径.
	void GetAllDefaultDownloadFile(TArray<XmlDownloadStruct *> & _downloadFileArray);

	FString GetLocalFile(XmlDownloadStruct * _serverPath);

	// 判断文件是否存在.
	bool CheckFileExits(const FString & _filePath);

	// 判断所有的MD5
	void CheckMd5(TArray<XmlDownloadStruct *> & _downloadArray,TArray<XmlDownloadStruct *> & _outArray);

	// 开始递归下载.
	void StartDownload();

	// xml data下载成功.
	void OnGetXmlContent(msg_ptr _msg);

	// 下载完的json保存到本地.
	void SaveDownloadJson(const FString & _content, const FString & _path);

	// 加载本地json文件到内存中.
	void LoadLocalJson(XmlDownloadStruct * _localJson);

	void LoadJsonByCode(TSharedPtr<FJsonObject> & mJsonObject,const FString & _jsonCode);

	// 获取套件详情数据的方法.
	void GetSuitDetailDownloadFile(TArray<XmlDownloadStruct *> & _downloadFileArray,int32 _suitId);
};
