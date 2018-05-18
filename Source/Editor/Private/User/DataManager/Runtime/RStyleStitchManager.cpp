// Fill out your copyright notice in the Description page of Project Settings.

#include "RStyleStitchManager.h"
#include "StyleStitchR.h"

RStyleStitchManager *   RStyleStitchManager::m_pInstance = nullptr;

RStyleStitchManager * RStyleStitchManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStyleStitchManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RStyleStitchManager::RStyleStitchManager()
{

}

RStyleStitchManager::~RStyleStitchManager()
{

}

void RStyleStitchManager::OnLoad()
{

}

void RStyleStitchManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStyleStitchManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	TArray<TSharedPtr<FJsonValue>> mStyleAccessory = _content->GetArrayField("styleStitchsuit");

	for (int32 i = 0; i < mStyleAccessory.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStyleAccessory[i]->AsObject();

		StyleStitchR * mRdata = new StyleStitchR();

		mRdata->GLoad();

		mRdata->InitWithJson(mContentJson);

		AddData(mRdata);
	}
}
