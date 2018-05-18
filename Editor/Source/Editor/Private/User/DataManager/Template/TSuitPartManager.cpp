// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitPartManager.h"
#include "SuitPartT.h"

TSuitPartManager *   TSuitPartManager::m_pInstance = nullptr;

TSuitPartManager * TSuitPartManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitPartManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitPartManager::TSuitPartManager()
{
}

TSuitPartManager::~TSuitPartManager()
{
}

void TSuitPartManager::OnLoad()
{

}

void TSuitPartManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitPartManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitPartT * mData = new SuitPartT();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		AddData(mData);
	}
}
