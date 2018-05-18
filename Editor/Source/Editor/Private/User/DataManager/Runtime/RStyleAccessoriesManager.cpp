// Fill out your copyright notice in the Description page of Project Settings.

#include "RStyleAccessoriesManager.h"
#include "StyleAccessoriesR.h"

RStyleAccessoriesManager *   RStyleAccessoriesManager::m_pInstance = nullptr;

RStyleAccessoriesManager * RStyleAccessoriesManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStyleAccessoriesManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}


RStyleAccessoriesManager::RStyleAccessoriesManager()
{
}

RStyleAccessoriesManager::~RStyleAccessoriesManager()
{
}

void RStyleAccessoriesManager::OnLoad()
{

}

void RStyleAccessoriesManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStyleAccessoriesManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mStyleAccessory = _content->GetArrayField("styleAccessory");

	for (int32 i = 0; i < mStyleAccessory.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStyleAccessory[i]->AsObject();

		StyleAccessoriesR * mRdata = new StyleAccessoriesR();

		mRdata->GLoad();

		mRdata->InitWithJson(mContentJson);

		AddData(mRdata);
	}
}
