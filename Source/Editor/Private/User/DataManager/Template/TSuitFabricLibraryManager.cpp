// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitFabricLibraryManager.h"
#include "SuitFabricLibraryT.h"

TSuitFabricLibraryManager *   TSuitFabricLibraryManager::m_pInstance = nullptr;

TSuitFabricLibraryManager * TSuitFabricLibraryManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitFabricLibraryManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitFabricLibraryManager::TSuitFabricLibraryManager()
{
}

TSuitFabricLibraryManager::~TSuitFabricLibraryManager()
{
}

void TSuitFabricLibraryManager::OnLoad()
{

}

void TSuitFabricLibraryManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitFabricLibraryManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mJsonData = _content->GetArrayField("data");

	// 3.解析数据.
	for (int32 i = 0; i < mJsonData.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJson = mJsonData[i];

		TSharedPtr<FJsonObject> mContentJson = mJson->AsObject();

		SuitFabricLibraryT * mData = new SuitFabricLibraryT();

		mData->GLoad();

		mData->InitWithJson(mContentJson);

		// 添加到容器里.
		AddData(mData);
	}
}
