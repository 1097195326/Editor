// Fill out your copyright notice in the Description page of Project Settings.

#include "RStyleComponentManager.h"
#include "StyleComponentR.h"

RStyleComponentManager *   RStyleComponentManager::m_pInstance = nullptr;

RStyleComponentManager * RStyleComponentManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStyleComponentManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RStyleComponentManager::RStyleComponentManager()
{
}

RStyleComponentManager::~RStyleComponentManager()
{
}

void RStyleComponentManager::OnLoad()
{

}

void RStyleComponentManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStyleComponentManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mStyleAccessory = _content->GetArrayField("styleComponent");

	for (int32 i = 0; i < mStyleAccessory.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStyleAccessory[i]->AsObject();

		StyleComponentR * mRdata = new StyleComponentR();

		mRdata->GLoad();

		mRdata->InitWithJson(mContentJson);

		AddData(mRdata);
	}
}
