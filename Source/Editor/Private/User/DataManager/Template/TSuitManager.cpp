// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitManager.h"

TSuitManager *   TSuitManager::m_pInstance = nullptr;

TSuitManager * TSuitManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitManager::TSuitManager()
{
}

TSuitManager::~TSuitManager()
{
}

void TSuitManager::OnLoad()
{

}

void TSuitManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	// 反序列化数据并添加到管理器中.

	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitT * mData = new SuitT();
		
		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		AddData(mData);
	}
}

