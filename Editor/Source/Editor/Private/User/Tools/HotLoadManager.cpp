//
//  HotLoadManager.cpp
//  Editor
//
//  Created by hongxing zhang on 2018/2/23.
//  Copyright © 2018年 Epic Games, Inc. All rights reserved.
//

#include "HotLoadManager.h"
#include "Editor.h"
#include "GController.h"
#include "HaoshaSDKBPLibrary.h"
#include "GFileHelper.h"
#include "GPlatformFileManager.h"
#include "PakMountHelper.h"
#include "EditorDirector.h"
#include <iostream>
#include "GLanguageTool.h"
#include <string>
#include "LogicUpdateScene.h"
#include "EditorDirector.h"
#include "UpdateUIController.h"
#include "MsgCenter.h"
#include "UIManager.h"

#include <string>

using namespace std;

#define VERSION_XML_FILE_NAME FString(TEXT("Name"))
#define VERSION_XML_FILE_MD5 FString(TEXT("MD5"))
#define VERSION_XML_FILE_SIZE FString(TEXT("Size"))

// UObject(const FObjectInitializer& ObjectInitializer);



UHotLoadManager::UHotLoadManager() :
	VersionLocalXmlFile(TEXT("VERSION-LOCAL.xml")),
	VersionNetXmlFile(TEXT("VERSION-NET.xml")),
	VersionUrlRootDir(TEXT("")),
	VersionLocalRootDir(TEXT("Versions")),
	VersionXMLDownloadFinishFlag(false),
	VersionPakDownloadFinishFlag(false),
	CurrentDownloadNumUnbelive(0),
	LastDownloadSize(0),
	LastCalTime(0.f),
	m_isLoading(false),
	m_CurrentLoadingFile(nullptr)
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Hot Load Manager Constractor func"));
	}

	UWorld* const World = GetWorld();
	CreationTime = (World ? World->GetTimeSeconds() : 0.f);

}

UHotLoadManager * UHotLoadManager::Manager = nullptr;

UHotLoadManager::~UHotLoadManager()
{
	Manager = nullptr;
}
UHotLoadManager * UHotLoadManager::GetInstance()
{
	if (Manager == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("HOT LOAD MANAGER INIT"));

		Manager = NewObject<UHotLoadManager>(UEditorDirector::GetInstance());
		Manager->AddToRoot();
	}
	return Manager;
}
void UHotLoadManager::Init()
{
	if (GetWorld())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Hot Load  Manager Init ..."));
	}

	GetWorld()->GetTimerManager().SetTimer(m_LoadTimerHandler, this, &UHotLoadManager::Tick, 1.f, true);

}
void UHotLoadManager::Destroy()
{
	Manager->RemoveFromRoot();
	GetWorld()->GetTimerManager().ClearTimer(m_LoadTimerHandler);
}
void UHotLoadManager::Tick()
{
	//    Super::Tick(DeltaSeconds);
	if (!m_isLoading && !m_AllTask.empty())
	{
		IOSSDownload * download = OSSHelper::GetInstance()->GetOSSTool();

		if (download->CanDownload())
		{
			m_isLoading = true;
			m_CurrentLoadingFile = m_AllTask.front();

			FOnOSSFileDownloadProgressDelegate _Pro = FOnOSSFileDownloadProgressDelegate(this, &UHotLoadManager::OnDownloadFileProgress);
			FOnOSSRequestFinishedDelegate  _fini = FOnOSSRequestFinishedDelegate(this, &UHotLoadManager::OnDownloadFileFinish);
			download->DownloadFiles
			(
				m_CurrentLoadingFile->m_Parameters, _Pro, _fini
			);
		}
	}
}
float UHotLoadManager::GetGameTimeSinceCreation()
{
	if (UWorld* MyWorld = GetWorld())
	{
		return MyWorld->GetTimeSeconds() - CreationTime;
	}
	// return 0.f if GetWorld return's null
	else
	{
		return 0.f;
	}
}

//----------------

void UHotLoadManager::GetUpdateFiles(TArray< FString >& arr)
{

	GetDiffVersions(GetLocalVersionFilePath(1), GetNetVersionFilePath(1), arr);
}

void UHotLoadManager::VersionForcedUpdateCheck()
{

}

void UHotLoadManager::OpenURL(FString inURL)
{
	UHaoshaSDKBPLibrary::OpenURL(inURL);
}

void UHotLoadManager::GetDiffVersions(const FString & oldfile, const FString & newfile, TArray< FString >& arr)
{
	UE_LOG(LogTemp, Error, TEXT("===========开始读取xml111==========="));


	UE_LOG(LogTemp, Error, TEXT("===========oldfile===========%s"), *oldfile);
	UE_LOG(LogTemp, Error, TEXT("===========newfile===========%s"), *newfile);
	IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (mPlatformFile.FileExists(*oldfile))
	{
		UE_LOG(LogTemp, Log, TEXT("old file is exist "));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("old file is not  exist "));
	}
	if (mPlatformFile.FileExists(*newfile))
	{
		UE_LOG(LogTemp, Log, TEXT("new file is exist "));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("new file is not  exist "));
	}

	FString mOldXmlContent;
	FFileHelper::LoadFileToString(mOldXmlContent, *oldfile);
	FString mNewXmlContent;
	FFileHelper::LoadFileToString(mNewXmlContent, *newfile);


	UE_LOG(LogTemp, Error, TEXT("===========mOldXmlContent===========%s"), *mOldXmlContent);
	UE_LOG(LogTemp, Error, TEXT("===========mNewXmlContent===========%s"), *mNewXmlContent);

	// GXML.
	TiXmlDocument docold, docnew;

	std::string xmlContentold = TCHAR_TO_UTF8(*mOldXmlContent);
	std::string xmlContentnew = TCHAR_TO_UTF8(*mNewXmlContent);

	docold.Parse(xmlContentold.c_str());
	docnew.Parse(xmlContentnew.c_str());
	TiXmlElement* rootold = docold.RootElement();
	TiXmlElement* rootnew = docnew.RootElement();



	int size = 0;

	if (rootold != NULL && rootnew != NULL) {
		UE_LOG(LogTemp, Error, TEXT("===========开始读取xml222==========="));
		//getdifferent
		TMap<FString, FFileInfo> oldmap = GetFileMapFromXml(rootold);
		TMap<FString, FFileInfo> newmap = GetFileMapFromXml(rootnew);
		TMap<FString, FFileInfo> diff = GetDiffNewFromMapInfo(oldmap, newmap);
		for (auto& item : diff) {
			FString xmlPath = GetVersionXmlFilePath(item.Value.Name);
			if (mPlatformFile.FileExists(*xmlPath))
			{
				FString oldfile_md;
				if (FFileHelper::LoadFileToString(oldfile_md, *xmlPath) && item.Value.MD5 != oldfile_md)
				{
					size += item.Value.Size;
					arr.Add(item.Value.Name);
					m_loadFileMD.Add(item.Value.Name, item.Value.MD5);
				}
			}
			else
			{
				size += item.Value.Size;
				arr.Add(item.Value.Name);
				m_loadFileMD.Add(item.Value.Name, item.Value.MD5);
				UE_LOG(LogTemp, Error, TEXT("===========GetDiffVersions===========%s"), *item.Value.Name);
			}
		}
	}
	else {
		if (rootnew == NULL) {
			UE_LOG(LogTemp, Error, TEXT("===========rootnew is null==========="));
			return;
		}
		if (rootold == NULL) {
			TMap<FString, FFileInfo> newmap = GetFileMapFromXml(rootnew);
			for (auto& item : newmap) {
				FString xmlPath = GetVersionXmlFilePath(item.Value.Name);
				if (mPlatformFile.FileExists(*xmlPath))
				{
					FString oldfile_md;
					if (FFileHelper::LoadFileToString(oldfile_md, *xmlPath) && item.Value.MD5 != oldfile_md)
					{
						size += item.Value.Size;
						arr.Add(item.Value.Name);
						m_loadFileMD.Add(item.Value.Name, item.Value.MD5);
					}
				}
				else
				{
					size += item.Value.Size;
					arr.Add(item.Value.Name);
					m_loadFileMD.Add(item.Value.Name, item.Value.MD5);
					UE_LOG(LogTemp, Error, TEXT("===========GetDiffVersions2===========%s"), *item.Value.Name);
				}

			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("===========filesize=========%d"), size);
	m_CurrentSize = 0.f;
	m_TotalSize = size;
	//IsDownloadFinish();
	onVersionFileDownloadTotalSize.Broadcast(size);
	return;
}

TMap<FString, FFileInfo> UHotLoadManager::GetDiffNewFromMapInfo(const TMap<FString, FFileInfo>& oldsrc, const TMap<FString, FFileInfo>& newsrc)
{
	if (oldsrc.Num() > 0 && newsrc.Num() > 0) {
		TMap<FString, FFileInfo> result;
		for (auto &item : newsrc) {
			//1.先判断文件存不存在.2.在判断配置表的版本号对比.
			TArray<FString> files;
			PakMountHelper::GetFiles(FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir), files, false, TEXT("pak"));

			FString mPakPath = FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, item.Key);

			if (!files.Contains(GPlatformFileManager::ExtractIOSDir(*mPakPath)))
			{
				UE_LOG(LogTemp, Log, TEXT("zhx : files : %s"), *files[0]);
				result.Add(item.Key, item.Value);
			}
			else
			{
				if (oldsrc.Contains(item.Key)) {
					FFileInfo oldfile = *(oldsrc.Find(item.Key));
					FFileInfo newfile = item.Value;
					UE_LOG(LogTemp, Error, TEXT("===========GetDiffNewFromMapInfo 1===========%s"), *item.Value.Name);
					if (oldfile.MD5 != newfile.MD5)
					{
						// 线删除文件.
						IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
						if (!mPlatformFile.DeleteFile(*GPlatformFileManager::ExtractIOSDir(*mPakPath)))
						{
							GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Red, TEXT("Hot Load Delete File Fail"));
						}
						UE_LOG(LogTemp, Error, TEXT("===========GetDiffNewFromMapInfo 2 add ===========%s"), *item.Value.Name);
						result.Add(item.Key, item.Value);
					}
				}
				else
				{
					result.Add(item.Key, item.Value);
				}
			}

		}
		return result;
	}
	else {
		if (oldsrc.Num() > 0) return TMap<FString, FFileInfo>();
		if (newsrc.Num() > 0)return newsrc;
	}
	return TMap<FString, FFileInfo>();
}

void UHotLoadManager::GetVersionXML()
{
	FString mPresentPath = GFileHelper::ProjectPersistentDownloadDir();

	FString mSavePath = FString::Printf(TEXT("%s/Versions/"), *mPresentPath);

	GPlatformFileManager::RecursiveCreateDirectory(*mSavePath);

	VersionXMLDownloadFinishFlag = false;
	VersionPakDownloadFinishFlag = false;
	CurrentDownloadNumUnbelive = 0;
	LastDownloadSize = 0;
	LastCalTime = 0.f;


	UE_LOG(LogTemp, Log, TEXT("Begin check version... : %s"), *GetLocalVersionFilePath());
	if (GConfig == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CANNOT OPEN CONFIG FILE"));
		return;
	}

	FString versionUrl;
	if (!GConfig->GetString(TEXT("SETTING.Package"), TEXT("VersionXmlUrl"), versionUrl, GGameIni))
	{
		UE_LOG(LogTemp, Error, TEXT("cannot get versionfile path from setting.version secsion."));
		return;
	}

	if (!GConfig->GetString(TEXT("SETTING.Package"), TEXT("VersionRootPath"), VersionUrlRootDir, GGameIni))
	{
		UE_LOG(LogTemp, Error, TEXT("cannot get version root path from setting.version secsion."));
		return;
	}

	IOSSDownload * download = OSSHelper::GetInstance()->GetOSSTool();

	if (!download->CanDownload())
	{
		UE_LOG(LogTemp, Error, TEXT("DOWNLOAD Module Can not Download"));
		return;
	}
	TArray<FOssReqeustParameter> Parameters;

	Parameters.Add(FOssReqeustParameter(FPaths::Combine(VersionUrlRootDir, versionUrl), FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, VersionNetXmlFile)));
	FOnOSSFileDownloadProgressDelegate ProgressDelegate = FOnOSSFileDownloadProgressDelegate(this, &UHotLoadManager::OnCBGetVersionXMLProgress);
	FOnOSSRequestFinishedDelegate FinishedDelegate = FOnOSSRequestFinishedDelegate(this, &UHotLoadManager::OnCBGetVersionXML);

	DownloadFiles(Parameters, ProgressDelegate, FinishedDelegate);
}

// 下载配置文件完成.
void UHotLoadManager::OnCBGetVersionXML(bool res)
{
	IOSSDownload * download = OSSHelper::GetInstance()->GetOSSTool();
	if (download->CanDownload())
	{
		VersionXMLDownloadFinishFlag = false;
		GetUpdateFiles(PakFiles);

		UE_LOG(LogTemp, Error, TEXT("===========GetDiffNewFromMapInfo===========%d"), PakFiles.Num());

		if (PakFiles.Num() > 0)
		{
			/*for (FString mFilePath : PakFiles)
			{
			FString mFullPath = FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, mFilePath);
			_DeleteFile(*mFullPath);
			}*/
			DownloadVersionFiles(PakFiles);
		}
		else
		{
			// 没有可以更新的.
			CheckFinished(true);
		}
	}
	else
	{
		// 不能下载.一般不会进这里面.
		CheckFinished(true);
	}
}

void UHotLoadManager::OnCBGetVersionXMLProgress(FString inFileName, int32 inCurrentSize, int32 inTotalSize)
{
}

void UHotLoadManager::DownloadVersionFiles(const TArray<FString> &inFiles)
{
	if (VersionUrlRootDir.Len() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Version Root direction missing!"));
		return;
	}

	IOSSDownload * download = OSSHelper::GetInstance()->GetOSSTool();



	for (const FString &elem : inFiles)
	{
		TArray<FOssReqeustParameter> Parameters;
		Parameters.Add(FOssReqeustParameter(FPaths::Combine(VersionUrlRootDir, elem), FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, elem)));
		FOnOSSFileDownloadProgressDelegate _ProgressDelegate = FOnOSSFileDownloadProgressDelegate(this, &UHotLoadManager::OnCBDownloadVersionFileProgress);
		FOnOSSRequestFinishedDelegate FinishedDelegate = FOnOSSRequestFinishedDelegate(this, &UHotLoadManager::OnCBDownloadVersionFiles);

		DownloadFiles(Parameters, _ProgressDelegate, FinishedDelegate, elem);
	}

}
void UHotLoadManager::DownloadFiles(const TArray<FOssReqeustParameter> &inParameters, FOnOSSFileDownloadProgressDelegate &inDelegate, FOnOSSRequestFinishedDelegate &inFinishDelegate, FString xmlName)
{
	DownLoadFile * mFile = new DownLoadFile();
	mFile->m_Parameters = inParameters;
	mFile->m_ProgressDelegate = inDelegate;
	mFile->m_FinishedDelegate = inFinishDelegate;
	mFile->m_xmlName = xmlName;
	m_AllTask.push(mFile);
}
void UHotLoadManager::OnDownloadFileFinish(bool res)
{
	if (!res)
	{
		for (int i = 0; m_AllTask.size() > 0; ++i)
		{
			DownLoadFile * file = m_AllTask.front();
			delete file;
			m_AllTask.pop();
		}
		m_AllTask.empty();
		PakFiles.Empty();
		m_isLoading = false;
#if PLATFORM_WINDOWS
		OSSHelper::GetInstance()->Release();
		((UEditorDirector*)UEditorDirector::GetInstance())->InitOSS();
#endif
		UGameDirector::GetInstance()->OpenLevel(TEXT("UpdateLevel"));

		return;
	}
	if (CheckDownloadFileSucess())
	{
		m_AllTask.pop();
		m_CurrentLoadingFile->m_FinishedDelegate.Execute();

		delete m_CurrentLoadingFile;
	}
	m_CurrentLoadingFile = nullptr;
	m_isLoading = false;

}
void UHotLoadManager::OnDownloadFileProgress(FString inFileName, int32 inCurrentSize, int32 inTotalSize)
{
	m_CurrentLoadingFile->m_ProgressDelegate.Execute(inFileName, inCurrentSize, inTotalSize);
}

bool UHotLoadManager::IsDownloadFinish()
{
	UE_LOG(LogTemp, Error, TEXT("开始保存配置文件============="));
	VersionPakDownloadFinishFlag = true;
	FString str;
	FFileHelper::LoadFileToString(str, *GetNetVersionFilePath(true));
	FFileHelper::SaveStringToFile(str, *GetLocalVersionFilePath(true));

	IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (mPlatformFile.FileExists(*GetNetVersionFilePath(1)))
	{
		UE_LOG(LogTemp, Error, TEXT("server  xml 保存成功"));
	}

	if (mPlatformFile.FileExists(*GetLocalVersionFilePath(1)))
	{
		UE_LOG(LogTemp, Error, TEXT("local  xml 保存成功"));
	}

	m_CurrentSize = 0.f;
	//m_DownloadedFiles.Empty();
	m_loadFileMD.Empty();

	//FFileHelper::LoadFileToString(str, *GetNetVersionFilePath());
	//FFileHelper::SaveStringToFile(str, *GetLocalVersionFilePath());
	UE_LOG(LogTemp, Log, TEXT("SAVE net version file to local version file"));
	return true;
}

void UHotLoadManager::OnCBDownloadVersionFiles(bool res)
{
	//CheckFinished(true);
	if (m_AllTask.empty())
	{
		TArray<FString>  mFailFile;
		if (CheckDownloadAllSucess(mFailFile))
		{
			GetVersionXML();
		}
		else
		{
			DownloadVersionFiles(mFailFile);
		}
	}
}

void UHotLoadManager::OnCBDownloadVersionFileProgress(FString inFileName, int32 inCurrentSize, int32 inTotalSize)
{

	if (PakFiles.Num() == 0)
	{
		return;
	}

	static float downloadSpeed = 0;

	m_CurrentSize += inCurrentSize - LastDownloadSize;

	if (inTotalSize == inCurrentSize && inCurrentSize != 0)
	{
		++CurrentDownloadNumUnbelive;
		LastDownloadSize = 0;
		LastCalTime = GetGameTimeSinceCreation();

		return;
	}

	if (GetGameTimeSinceCreation() > LastCalTime && inCurrentSize > LastDownloadSize)
	{
		int32 deltaSize = LastDownloadSize == 0 ? inCurrentSize : inCurrentSize - LastDownloadSize;

		downloadSpeed = deltaSize / ((GetGameTimeSinceCreation() - LastCalTime) * 1024);

		LastDownloadSize = inCurrentSize;
		LastCalTime = GetGameTimeSinceCreation();
		if (downloadSpeed > 1000)
		{
			onVersionFileDownloadSpeed.Broadcast(FString::Printf(TEXT("%0.2fMB/s"), downloadSpeed / 1024.f));
		}
		else
		{
			onVersionFileDownloadSpeed.Broadcast(FString::Printf(TEXT("%0.2fKB/s"), downloadSpeed));
		}
	}
	UE_LOG(LogTemp, Log, TEXT("zhx : OnCBDownloadVersionFileProgress %s, %d, %d"), *inFileName, inCurrentSize, inTotalSize);
	UE_LOG(LogTemp, Log, TEXT("zhx : OnCBDownloadVersionFileProgress2 %d, %d"), CurrentDownloadNumUnbelive, PakFiles.Num());
	float tmp = (((float)CurrentDownloadNumUnbelive) / PakFiles.Num() + (((float)inCurrentSize) / inTotalSize) / PakFiles.Num());// *100;
																																 //    float tmp = m_CurrentSize / m_TotalSize;
	OnVersionDownloadTotalProgress.Broadcast(FString::Printf(TEXT("%0.2f"), tmp));

	DownlaodInfoStruct info;
	info.m_DownloadMsg = GLanguageTool::GetInstance()->GetLanguage(TEXT("key154"));
	info.m_Progress = tmp;
	info.m_Size = m_TotalSize / 1024.f / 1024.f;
	info.m_Speed = downloadSpeed;

	msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, LogicUpdateScene::m_DownloadPak_Progress, &info));
	MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);
}



bool UHotLoadManager::DownloadCheckPak() {
	TArray<FString> files;
	PakMountHelper::GetFiles(FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir), files, false, TEXT("pak"));
	FString mNewXmlContent;
	FFileHelper::LoadFileToString(mNewXmlContent, *GetNetVersionFilePath(1));
	TiXmlDocument docnew;
	std::string xmlContentnew = TCHAR_TO_UTF8(*mNewXmlContent);
	docnew.Parse(xmlContentnew.c_str());
	TiXmlElement* root = docnew.RootElement();
	if (root == NULL)return false;
	TArray<FString> xmlfiles = GetFileNameFromXml(root);
	FString tmpFileCleanName;
	if (xmlfiles.Num() < 0)return false;
	for (int i = 0; i < xmlfiles.Num(); i++)
	{
		bool downloadok = false;
		for (int k = 0; k < files.Num(); k++)
		{
			if (files[k].Contains(xmlfiles[i])) {
				downloadok = true;
				break;
			}
		}
		if (downloadok == false) return false;
	}
	return true;

}
bool UHotLoadManager::CheckDownloadFileSucess()
{
	bool _res = false;

	IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	//    for(FOssReqeustParameter par : m_CurrentLoadingFile->m_Parameters)
	//    {
	//        FString mFilePath = par.LocalPath;
	//        if (GPlatformFileManager::FileExists(*mFilePath))
	//        {
	//            _res = true;
	//        }else
	//        {
	//            _res = false;
	//            break;
	//        }
	//    }
	FString mFilePath = m_CurrentLoadingFile->m_Parameters[0].LocalPath;
	if (mPlatformFile.FileExists(*GPlatformFileManager::ExtractIOSDir(*mFilePath)))
	{
		FString _xmlName = m_CurrentLoadingFile->m_xmlName;
		if (_xmlName.Len() > 0)
		{
			//m_DownloadedFiles.Add(_xmlName);

			FString mPath = GetVersionXmlFilePath(_xmlName);
			UE_LOG(LogTemp, Log, TEXT("zhx : GetVersionXmlFilePath: %s"), *mPath);
			FFileHelper::SaveStringToFile(m_loadFileMD[_xmlName], *mPath);
		}
		_res = true;
	}

	return _res;
}
bool UHotLoadManager::CheckDownloadAllSucess(TArray<FString> & arr)
{
	bool _res = true;
	IPlatformFile& mPlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	for (FString mPath : PakFiles)
	{
		FString mFullPath = FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, mPath);
		if (!mPlatformFile.FileExists(*GPlatformFileManager::ExtractIOSDir(*mFullPath)))
		{
			arr.Add(mPath);
			_res = false;
		}
	}
	if (_res)
	{
		PakFiles.Empty();
		IsDownloadFinish();
	}
	return _res;
}

void UHotLoadManager::CheckFinished(bool inSuccess)
{

	UE_LOG(LogTemp, Error, TEXT("开始CheckFinished==="));
	UEditorDirector *inst = Cast<UEditorDirector>(GetWorld()->GetGameInstance());
	if (inst == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game instance error when try to mount local pak"));
	}
	else
	{
		//if (DownloadCheckPak() == false) {
		// 更新失败.临时让他掉线..
		//inst->CloseGame();
		//GetVersionXML();
		//return;
		//}

		inst->MountLocalPakFiles(FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir));
	}
	UE_LOG(LogTemp, Error, TEXT("结束CheckFinished==="));
	OnVersionCheckFinished.Broadcast(inSuccess);

	msg_ptr mBrodcastForcedResponse(new MsgLocal(MessageTypeLocal, LogicUpdateScene::m_DownloadPak_Finished, nullptr));
	MsgCenter::Instance()->SendMsg(mBrodcastForcedResponse);

}
void UHotLoadManager::PakMount()
{
	UEditorDirector *inst = Cast<UEditorDirector>(GetWorld()->GetGameInstance());
	if (inst == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game instance error when try to mount local pak"));
	}
	else
	{
		inst->MountLocalPakFiles(FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir));
	}
}
TMap<FString, FFileInfo> UHotLoadManager::GetFileMapFromXml(TiXmlElement * xmlobj)
{
	TMap<FString, FFileInfo> result;
	for (TiXmlElement* pNodeChild = xmlobj->FirstChildElement(); NULL != pNodeChild; pNodeChild = pNodeChild->NextSiblingElement()) {

		FFileInfo info;
		std::string name0 = TCHAR_TO_UTF8(*VERSION_XML_FILE_NAME);
		std::string name1 = TCHAR_TO_UTF8(*VERSION_XML_FILE_MD5);
		std::string name2 = TCHAR_TO_UTF8(*VERSION_XML_FILE_SIZE);
		const char* att0 = pNodeChild->Attribute(name0.c_str());
		const FString name = UTF8_TO_TCHAR(att0);// Attribute(TCHAR_TO_UTF8(*VERSION_XML_FILE_NAME));
		const char* att1 = pNodeChild->Attribute(name1.c_str());
		const char* att2 = pNodeChild->Attribute(name2.c_str());
		const FString md5 = UTF8_TO_TCHAR(att1); //pNodeChild->Attribute(TCHAR_TO_UTF8(*VERSION_XML_FILE_MD5));
		const FString sizestr = UTF8_TO_TCHAR(att2); //pNodeChild->Attribute(TCHAR_TO_UTF8(*VERSION_XML_FILE_SIZE));
		int size = FCString::Atoi(*sizestr);
		info.Name = name;
		info.MD5 = md5;
		info.Size = size;
		result.Add(name, info);
	}
	return result;
}
TArray<FString> UHotLoadManager::GetFileNameFromXml(TiXmlElement * xmlobj)
{
	TArray<FString> result;
	for (TiXmlElement* pNodeChild = xmlobj->FirstChildElement(); NULL != pNodeChild; pNodeChild = pNodeChild->NextSiblingElement()) {
		FFileInfo info;
		std::string name0 = TCHAR_TO_UTF8(*VERSION_XML_FILE_NAME);
		const char* att0 = pNodeChild->Attribute(name0.c_str());
		const FString name = UTF8_TO_TCHAR(att0);// Attribute(TCHAR_TO_UTF8(*VERSION_XML_FILE_NAME));
		result.Add(name);
	}
	return result;
}

FString UHotLoadManager::GetLocalVersionFilePath(bool IsExtract)
{
	FString mPath = FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, VersionLocalXmlFile);
	GPlatformFileManager::RecursiveCreateDirectory(*mPath);
	if (GController::m_platform == ENUM_PLATFORM::GIOS && IsExtract)
	{
		return GPlatformFileManager::ExtractIOSDir(*mPath);
	}
	return mPath;
}

FString UHotLoadManager::GetNetVersionFilePath(bool IsExtract)
{
	FString mPath = FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, VersionNetXmlFile);
	GPlatformFileManager::RecursiveCreateDirectory(*mPath);
	if (GController::m_platform == ENUM_PLATFORM::GIOS && IsExtract)
	{
		return GPlatformFileManager::ExtractIOSDir(*mPath);
	}
	return mPath;
}
FString UHotLoadManager::GetVersionXmlFilePath(FString fileName)
{
	FString left;

	if (fileName.Split(".", &left, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
	{

	}
	left.Append(TEXT(".xml"));

	FString mPath = FPaths::Combine(GFileHelper::ProjectHotloadDownloadDir(), VersionLocalRootDir, left);

	return GPlatformFileManager::ExtractIOSDir(*mPath);

}


