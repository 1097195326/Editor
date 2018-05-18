// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitComponentT.h"

SuitComponentT::SuitComponentT()
{
}

SuitComponentT::~SuitComponentT()
{
}

void SuitComponentT::OnLoad()
{

}

void SuitComponentT::OnRelease()
{

}

void SuitComponentT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_AdapterType= _jsonObj->GetIntegerField(TEXT("adapterType"));
	m_CameraRotate = _jsonObj->GetNumberField(TEXT("cameraRotate"));
	m_cameraDistance = _jsonObj->GetNumberField(TEXT("cameraDistance"));
	m_cameraHeight = _jsonObj->GetNumberField(TEXT("cameraHeight"));
	m_cameraPitching = _jsonObj->GetNumberField(TEXT("cameraPitching"));
	m_IconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));

	const TArray<TSharedPtr<FJsonValue>>& mSuitAccessoriesIdsArray = _jsonObj->GetArrayField(TEXT("suitAccessoriesIds"));
	for (int32 i = 0; i < mSuitAccessoriesIdsArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mTempJson = mSuitAccessoriesIdsArray[i];

		int32 mPartId = mTempJson->AsNumber();

		m_SuitAccessoriesIds.Add(mPartId);
	}
	

	const TArray<TSharedPtr<FJsonValue>>& mSuitSliceIdsArray = _jsonObj->GetArrayField(TEXT("suitSliceIds"));
	for (int32 i = 0; i < mSuitSliceIdsArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mTempJson = mSuitSliceIdsArray[i];

		int32 mPartId = mTempJson->AsNumber();

		m_SuitSliceIds.Add(mPartId);
	}


	const TArray<TSharedPtr<FJsonValue>>& mStitchsuitIdsArray = _jsonObj->GetArrayField("suitStitchsuitIds");
	for (int32 i = 0; i < mStitchsuitIdsArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mTempJson = mStitchsuitIdsArray[i];

		int32 mPartId = mTempJson->AsNumber();

		m_SuitStitchsuitIds.Add(mPartId);
	}
	
}
