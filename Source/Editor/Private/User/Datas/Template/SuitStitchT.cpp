// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitStitchT.h"

SuitStitchT::SuitStitchT()
{
}

SuitStitchT::~SuitStitchT()
{
}

void SuitStitchT::OnLoad()
{

}

void SuitStitchT::OnRelease()
{

}

void SuitStitchT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Status = _jsonObj->GetIntegerField(TEXT("status"));
	m_IconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));
	m_IsMainPart = _jsonObj->GetIntegerField(TEXT("isMainPart"));
	const TArray<TSharedPtr<FJsonValue>>& mTempIdArray = _jsonObj->GetArrayField(TEXT("suitComponentIds"));

	for (int32 i = 0; i < mTempIdArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mTempJson = mTempIdArray[i];

		int32 mPartId = mTempJson->AsNumber();

		m_SuitComponentIds.Add(mPartId);
	}

}
