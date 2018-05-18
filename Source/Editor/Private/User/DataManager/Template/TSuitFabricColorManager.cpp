// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitFabricColorManager.h"
#include "SuitFabricColorT.h"

TSuitFabricColorManager *   TSuitFabricColorManager::m_pInstance = nullptr;

TSuitFabricColorManager * TSuitFabricColorManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitFabricColorManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitFabricColorManager::TSuitFabricColorManager()
{
}

TSuitFabricColorManager::~TSuitFabricColorManager()
{
}

void TSuitFabricColorManager::OnLoad()
{

}

void TSuitFabricColorManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitFabricColorManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mJsonData = _content->GetArrayField("data");

	// 3.解析数据.
	for (int32 i = 0; i < mJsonData.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJson = mJsonData[i];

		TSharedPtr<FJsonObject> mContentJson = mJson->AsObject();

		SuitFabricColorT * mData = new SuitFabricColorT();

		mData->GLoad();

		mData->InitWithJson(mContentJson);

		// 添加到容器里.
		AddData(mData);
	}
}
