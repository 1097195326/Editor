// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitAccessoriesManager.h"
#include "SuitAccessoriesT.h"

TSuitAccessoriesManager *   TSuitAccessoriesManager::m_pInstance = nullptr;

TSuitAccessoriesManager * TSuitAccessoriesManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitAccessoriesManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitAccessoriesManager::TSuitAccessoriesManager()
{
}

TSuitAccessoriesManager::~TSuitAccessoriesManager()
{
}

void TSuitAccessoriesManager::OnLoad()
{

}

void TSuitAccessoriesManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitAccessoriesManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitAccessoriesT * mData = new SuitAccessoriesT();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		AddData(mData);
	}
}
