// Fill out your copyright notice in the Description page of Project Settings.

#include "RStyleSliceManager.h"
#include "StyleSliceR.h"

RStyleSliceManager *   RStyleSliceManager::m_pInstance = nullptr;

RStyleSliceManager * RStyleSliceManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStyleSliceManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RStyleSliceManager::RStyleSliceManager()
{
}

RStyleSliceManager::~RStyleSliceManager()
{
}

void RStyleSliceManager::OnLoad()
{

}

void RStyleSliceManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStyleSliceManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mStyleAccessory = _content->GetArrayField("styleSlice");

	for (int32 i = 0; i < mStyleAccessory.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStyleAccessory[i]->AsObject();

		StyleSliceR * mRdata = new StyleSliceR();

		mRdata->GLoad();

		mRdata->InitWithJson(mContentJson);

		AddData(mRdata);
	}
}
