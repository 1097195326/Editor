// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitComponentManager.h"
#include "SuitComponentT.h"

TSuitComponentManager *   TSuitComponentManager::m_pInstance = nullptr;

TSuitComponentManager * TSuitComponentManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitComponentManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitComponentManager::TSuitComponentManager()
{
}

TSuitComponentManager::~TSuitComponentManager()
{
}

void TSuitComponentManager::OnLoad()
{

}

void TSuitComponentManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitComponentManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitComponentT * mData = new SuitComponentT();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		AddData(mData);
	}
}
