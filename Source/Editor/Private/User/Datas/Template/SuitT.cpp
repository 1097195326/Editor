// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitT.h"

SuitT::SuitT()
{
}
SuitT::~SuitT()
{
}

void SuitT::OnLoad()
{

}

void SuitT::OnRelease()
{

}

void SuitT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_catId = _jsonObj->GetIntegerField(TEXT("catId"));

	m_iconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));

	m_parentCatId = _jsonObj->GetIntegerField(TEXT("parentCatId"));

	m_showModel = _jsonObj->GetStringField(TEXT("showModel"));

	m_sort = _jsonObj->GetIntegerField(TEXT("sort"));

	m_status = _jsonObj->GetIntegerField(TEXT("sort"));

	m_tableModel = _jsonObj->GetStringField(TEXT("tableModel"));

	_jsonObj->TryGetStringField(TEXT("catwalk"),m_catwalk);

	_jsonObj->TryGetStringField(TEXT("design"), m_design);
	
	_jsonObj->TryGetStringField(TEXT("showAnimation"), m_showModelAnimation);

	_jsonObj->TryGetStringField(TEXT("walkSequence"), m_walkSequence);

	const TArray<TSharedPtr<FJsonValue>>& mPartIdArray = _jsonObj->GetArrayField(TEXT("suitParts"));

	_jsonObj->TryGetStringArrayField(TEXT("defaultModel"), m_defaultModelArray);

	for (int32 i = 0; i < mPartIdArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mStyleJson = mPartIdArray[i];

		int32 mPartId = mStyleJson->AsNumber();

		m_ArrayPartId.Add(mPartId);
	}

	const TSharedPtr<FJsonObject> & mCameraSettingJson = _jsonObj->GetObjectField(TEXT("cameraMap"));

	const TSharedPtr<FJsonObject> & mCameraPhotoJson = mCameraSettingJson->GetObjectField(TEXT("cover"));

	m_cameraPhotoCover.m_cameraDistance = mCameraPhotoJson->GetNumberField(TEXT("cameraDistance"));

	m_cameraPhotoCover.m_cameraHeight = mCameraPhotoJson->GetNumberField(TEXT("cameraHeight"));

	m_cameraPhotoCover.m_rotateAngle = mCameraPhotoJson->GetNumberField(TEXT("rotateAngle"));

	m_cameraPhotoCover.m_cameraPitching = mCameraPhotoJson->GetNumberField(TEXT("cameraPitching"));

	m_cameraPhotoCover.m_isDisplayModel = mCameraPhotoJson->GetNumberField(TEXT("isDisplayModel"));

	const TSharedPtr<FJsonObject> & mCameraDesignJson = mCameraSettingJson->GetObjectField(TEXT("scene"));

	m_cameraDesignCover.m_cameraDistance = mCameraDesignJson->GetNumberField(TEXT("cameraDistance"));

	m_cameraDesignCover.m_cameraHeight = mCameraDesignJson->GetNumberField(TEXT("cameraHeight"));

	m_cameraDesignCover.m_rotateAngle = mCameraDesignJson->GetNumberField(TEXT("rotateAngle"));

	m_cameraDesignCover.m_cameraPitching = mCameraDesignJson->GetNumberField(TEXT("cameraPitching"));

	m_cameraDesignCover.m_isDisplayModel = mCameraDesignJson->GetNumberField(TEXT("isDisplayModel"));
}


