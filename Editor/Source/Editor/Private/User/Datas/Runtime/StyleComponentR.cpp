// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleComponentR.h"

StyleComponentR::StyleComponentR()
{
}

StyleComponentR::~StyleComponentR()
{
}

void StyleComponentR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("suitComponentId"));

	// 辅料.
	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _jsonObj->GetArrayField(TEXT("styleAccessoryIds"));

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJsonData = mContentArray[i];

		m_styleAccessoryIds.Add(mJsonData->AsNumber());
	}

	// 片.
	const TArray<TSharedPtr<FJsonValue>>& mSliceArrar = _jsonObj->GetArrayField(TEXT("styleSliceIds"));

	for (int32 i = 0; i < mSliceArrar.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJsonData = mSliceArrar[i];

		m_styleSliceIds.Add(mJsonData->AsNumber());
	}

	// 线迹.
	const TArray<TSharedPtr<FJsonValue>>& mStitchArray = _jsonObj->GetArrayField(TEXT("styleStitchsuitIds"));

	for (int32 i = 0; i < mStitchArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJsonData = mStitchArray[i];

		m_styleStitchsuitIds.Add(mJsonData->AsNumber());
	}

}

void StyleComponentR::OnLoad()
{

}

void StyleComponentR::OnRelease()
{

}
