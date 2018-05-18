// Fill out your copyright notice in the Description page of Project Settings.

#include "RStylePatternManager.h"
#include "StylePatternR.h"

RStylePatternManager *   RStylePatternManager::m_pInstance = nullptr;

RStylePatternManager * RStylePatternManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStylePatternManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RStylePatternManager::RStylePatternManager()
{
}

RStylePatternManager::~RStylePatternManager()
{
}

void RStylePatternManager::OnLoad()
{

}

void RStylePatternManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStylePatternManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mStyleAccessory = _content->GetArrayField("stylePattern");

	for (int32 i = 0; i < mStyleAccessory.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStyleAccessory[i]->AsObject();

		StylePatternR * mRdata = new StylePatternR();

		mRdata->GLoad();

		mRdata->InitWithJson(mContentJson);

		AddData(mRdata);
	}
}
