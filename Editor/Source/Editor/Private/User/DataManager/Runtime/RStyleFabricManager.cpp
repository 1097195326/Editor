// Fill out your copyright notice in the Description page of Project Settings.

#include "RStyleFabricManager.h"
#include "StyleFabricR.h"

RStyleFabricManager *   RStyleFabricManager::m_pInstance = nullptr;

RStyleFabricManager * RStyleFabricManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStyleFabricManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RStyleFabricManager::RStyleFabricManager()
{
}

RStyleFabricManager::~RStyleFabricManager()
{
}

void RStyleFabricManager::OnLoad()
{

}

void RStyleFabricManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStyleFabricManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mStyleAccessory = _content->GetArrayField("styleFabric");

	for (int32 i = 0; i < mStyleAccessory.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStyleAccessory[i]->AsObject();

		StyleFabricR * mRdata = new StyleFabricR();

		mRdata->GLoad();

		mRdata->InitWithJson(mContentJson);

		AddData(mRdata);
	}
}
