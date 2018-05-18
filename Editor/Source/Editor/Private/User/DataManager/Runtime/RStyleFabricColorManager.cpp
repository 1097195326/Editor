// Fill out your copyright notice in the Description page of Project Settings.

#include "RStyleFabricColorManager.h"
#include "StyleFabricColorR.h"

RStyleFabricColorManager *   RStyleFabricColorManager::m_pInstance = nullptr;

RStyleFabricColorManager * RStyleFabricColorManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStyleFabricColorManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RStyleFabricColorManager::RStyleFabricColorManager()
{
}

RStyleFabricColorManager::~RStyleFabricColorManager()
{
}

void RStyleFabricColorManager::OnLoad()
{

}

void RStyleFabricColorManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStyleFabricColorManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mStyleAccessory = _content->GetArrayField("styleColor");

	for (int32 i = 0; i < mStyleAccessory.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStyleAccessory[i]->AsObject();

		StyleFabricColorR * mRdata = new StyleFabricColorR();

		mRdata->GLoad();

		mRdata->InitWithJson(mContentJson);

		AddData(mRdata);
	}
}
