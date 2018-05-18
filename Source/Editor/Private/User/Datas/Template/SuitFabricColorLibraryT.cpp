 // Fill out your copyright notice in the Description page of Project Settings.

#include "SuitFabricColorLibraryT.h"
#include "SuitFabricColorT.h"
#include "TSuitFabricColorManager.h"

SuitFabricColorLibraryT::SuitFabricColorLibraryT()
{
}

SuitFabricColorLibraryT::~SuitFabricColorLibraryT()
{
}

void SuitFabricColorLibraryT::OnLoad()
{

}

void SuitFabricColorLibraryT::OnRelease()
{

}

void SuitFabricColorLibraryT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_iconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));

	const TArray<TSharedPtr<FJsonValue>>& mJsonArray = _jsonObj->GetArrayField(TEXT("pantoneLib"));

	for (int32 i = 0; i < mJsonArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJson = mJsonArray[i];

		const TSharedPtr<FJsonObject>& mStyleObject = mJson->AsObject();

		// 创建花色.
		SuitFabricColorT *mColor = new SuitFabricColorT();

		mColor->InitWithJson(mStyleObject);
	
		TSuitFabricColorManager::GetInstance()->AddData(mColor);

		m_fabricColorIds.Add(mColor->GetTid());
	}
}
