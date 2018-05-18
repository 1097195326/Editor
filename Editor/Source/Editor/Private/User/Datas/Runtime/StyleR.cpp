
// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleR.h"

StyleR::StyleR()
{
}

StyleR::~StyleR()
{
}

void StyleR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	_jsonObj->TryGetStringField(TEXT("iconUrl"), m_iconUrl);

	_jsonObj->TryGetStringField(TEXT("commont"),m_commont);

	_jsonObj->TryGetNumberField(TEXT("isRecommend"),m_isRecommend);

	_jsonObj->TryGetNumberField(TEXT("suitId"), m_Tid);

	UE_LOG(LogTemp, Log, TEXT("suit id : %d"), m_Tid);

	TArray<TSharedPtr<FJsonValue>> mStylePartIdsArr = _jsonObj->GetArrayField(TEXT("stylePartIds"));

	for (int32 i=0;i<mStylePartIdsArr.Num();i++)
	{
		int32 mStylePartId = mStylePartIdsArr[i]->AsNumber();

		m_stylePartIdArray.Add(mStylePartId);
	}
}

void StyleR::OnLoad()
{

}

void StyleR::OnRelease()
{

}
