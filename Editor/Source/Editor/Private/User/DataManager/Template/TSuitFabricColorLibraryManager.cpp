// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitFabricColorLibraryManager.h"
#include "SuitFabricColorLibraryT.h"
#include "TSuitFabricColorManager.h"

TSuitFabricColorLibraryManager *   TSuitFabricColorLibraryManager::m_pInstance = nullptr;

TSuitFabricColorLibraryManager * TSuitFabricColorLibraryManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitFabricColorLibraryManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitFabricColorLibraryManager::TSuitFabricColorLibraryManager()
{
}

TSuitFabricColorLibraryManager::~TSuitFabricColorLibraryManager()
{
}

void TSuitFabricColorLibraryManager::OnLoad()
{

}

void TSuitFabricColorLibraryManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitFabricColorLibraryManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mJsonData = _content->GetArrayField("data");

	// 3.解析数据.
	for (int32 i = 0; i < mJsonData.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJson = mJsonData[i];

		TSharedPtr<FJsonObject> mContentJson = mJson->AsObject();

		// 花色库.
		SuitFabricColorLibraryT * mData = new SuitFabricColorLibraryT();

		mData->GLoad();

		mData->InitWithJson(mContentJson);

		// 添加到容器里.
		AddData(mData);
	}
}

bool TSuitFabricColorLibraryManager::CheckFabricColorInLibrary(const FString & _sn, int32 _colorLibTid)
{
	SuitFabricColorLibraryT * mFabricColorLib = GetTemplateByPID<SuitFabricColorLibraryT>(_colorLibTid);

	for (int32 i=0;i<mFabricColorLib->m_fabricColorIds.Num();i++)
	{
		SuitFabricColorT * mColorT = TSuitFabricColorManager::GetInstance()->GetTemplateByPID<SuitFabricColorT>(mFabricColorLib->m_fabricColorIds[i]);
		if (mColorT->m_PantoneCode.Equals(_sn))
		{
			return true;
		}
	}
	return false;
}
