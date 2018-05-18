// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitPartT.h"

SuitPartT::SuitPartT()
{
}

SuitPartT::~SuitPartT()
{
}

void SuitPartT::OnLoad()
{

}

void SuitPartT::OnRelease()
{

}

void SuitPartT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{

	m_IconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));

	m_IsMainPart = _jsonObj->GetIntegerField(TEXT("isMainPart"));


	m_Status = _jsonObj->GetIntegerField(TEXT("status"));

	m_SuitComponentLibraryId = _jsonObj->GetIntegerField(TEXT("suitComponentLibraryId"));

	 
	const TArray<TSharedPtr<FJsonValue>>& mTempArray = _jsonObj->GetArrayField(TEXT("suitComponentIds"));

	for (int32 i = 0; i < mTempArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mTempArray[i];

		int32 mPartId = mStyleJson->AsNumber();

		m_SuitComponentIds.Add(mPartId);
	}
}
