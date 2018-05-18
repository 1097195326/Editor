// Fill out your copyright notice in the Description page of Project Settings.

#include "TSuitStitchManager.h"
#include "SuitStitchT.h"

TSuitStitchManager *   TSuitStitchManager::m_pInstance = nullptr;

TSuitStitchManager * TSuitStitchManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new TSuitStitchManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}


TSuitStitchManager::TSuitStitchManager()
{
}

TSuitStitchManager::~TSuitStitchManager()
{
}

void TSuitStitchManager::OnLoad()
{

}

void TSuitStitchManager::OnRelease()
{
	m_pInstance = nullptr;
}

void TSuitStitchManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _content->GetArrayField("data");

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mContentArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mStyleJson->AsObject();

		SuitStitchT * mData = new SuitStitchT();

		mData->GLoad();

		mData->InitWithJson(mStyleObject);

		// 添加到容器里.
		AddData(mData);
	}
}
