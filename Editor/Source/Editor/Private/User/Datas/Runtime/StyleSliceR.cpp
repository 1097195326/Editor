// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleSliceR.h"

StyleSliceR::StyleSliceR()
{
}

StyleSliceR::~StyleSliceR()
{
}

void StyleSliceR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("suitSliceId"));

	m_suitFabricId = _jsonObj->GetIntegerField(TEXT("styleFabricId"));

	m_suitPatternId = _jsonObj->GetStringField(TEXT("stylePattern"));

	m_suitColorId = _jsonObj->GetIntegerField(TEXT("colorId"));

	const TArray<TSharedPtr<FJsonValue>>& mContentArray = _jsonObj->GetArrayField(TEXT("styleAccessoryIds"));

	for (int32 i = 0; i < mContentArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mJsonData = mContentArray[i];

		m_styleAccessoryIds.Add(mJsonData->AsNumber());
	}

	m_angle = _jsonObj->GetNumberField(TEXT("angle"));

	TSharedPtr<FJsonObject> mJsonOffset = _jsonObj->GetObjectField(TEXT("offSet"));

	m_offset.X = mJsonOffset->GetNumberField(TEXT("x"));

	m_offset.Y = mJsonOffset->GetNumberField(TEXT("y"));

	TSharedPtr<FJsonObject> mJsonUvScale = _jsonObj->GetObjectField(TEXT("uVScaleUser"));

	m_uVScaleUser.X = mJsonOffset->GetNumberField(TEXT("x"));

	m_uVScaleUser.Y = mJsonOffset->GetNumberField(TEXT("y"));
}

void StyleSliceR::OnLoad()
{

}

void StyleSliceR::OnRelease()
{

}
