// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitFabricLibraryT.h"

SuitFabricLibraryT::SuitFabricLibraryT()
{
}

SuitFabricLibraryT::~SuitFabricLibraryT()
{
}

void SuitFabricLibraryT::OnLoad()
{

}

void SuitFabricLibraryT::OnRelease()
{

}

void SuitFabricLibraryT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_IconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));

	const TArray<TSharedPtr<FJsonValue>>& mTempArray = _jsonObj->GetArrayField(TEXT("fabrics"));

	for (int32 i = 0; i < mTempArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mTempArray[i];

		int32 mPartId = mStyleJson->AsNumber();

		m_Fabrics.Add(mPartId);
	}
}
