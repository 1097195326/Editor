// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitAccessoriesT.h"

SuitAccessoriesT::SuitAccessoriesT()
{
	 
}

SuitAccessoriesT::~SuitAccessoriesT()
{
}

void SuitAccessoriesT::OnLoad()
{

}

void SuitAccessoriesT::OnRelease()
{

}

void SuitAccessoriesT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_CameraRotate = _jsonObj->GetStringField(TEXT("cameraRotate"));
	m_Code = _jsonObj->GetStringField(TEXT("code"));
	m_IconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));
	m_ImageUrl = _jsonObj->GetStringField(TEXT("imageUrl"));
	m_State = _jsonObj->GetStringField(TEXT("state"));
	m_Status = _jsonObj->GetIntegerField(TEXT("status"));
	useType=_jsonObj->GetStringField(TEXT("useType"));
	
	const TArray<TSharedPtr<FJsonValue>>& mSuitPartIdsArray = _jsonObj->GetArrayField(TEXT("suitComponentIds"));
	for (int32 i = 0; i < mSuitPartIdsArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mTempJson = mSuitPartIdsArray[i];
		int32 mPartId = mTempJson->AsNumber();
		m_SuitComponent.Add(mPartId);
	}
}
