// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitSliceManager.h"
#include "SuitSliceT.h"

TSuitSliceManager *   TSuitSliceManager::m_pInstance = nullptr;

TSuitSliceManager * TSuitSliceManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitSliceManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

TSuitSliceManager::TSuitSliceManager()
{
}

TSuitSliceManager::~TSuitSliceManager()
{
}

void TSuitSliceManager::OnLoad()
{

}

void TSuitSliceManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitSliceManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitSliceT * mData = new SuitSliceT();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		AddData(mData);
	}
}
