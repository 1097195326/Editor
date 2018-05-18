// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitSliceT.h"

SuitSliceT::SuitSliceT()
{
}

SuitSliceT::~SuitSliceT()
{
}

void SuitSliceT::OnLoad()
{

}

void SuitSliceT::OnRelease()
{

}

void SuitSliceT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_CameraRotate = _jsonObj->GetNumberField(TEXT("cameraRotate"));
	m_cameraDistance = _jsonObj->GetNumberField(TEXT("cameraDistance"));
	m_cameraHeight = _jsonObj->GetNumberField(TEXT("cameraHeight"));
	m_cameraPitching = _jsonObj->GetNumberField(TEXT("cameraPitching"));

	_jsonObj->TryGetNumberField(TEXT("fabricLibId"), m_FabricLibId);
	_jsonObj->TryGetStringField(TEXT("meshUrl"), m_MeshUrl);
	m_OriginalSize = _jsonObj->GetNumberField(TEXT("originalSize"));
	_jsonObj->TryGetStringField(TEXT("patternDefaultId"), m_PatternDefaultId);
	_jsonObj->TryGetStringField(TEXT("baseTextureUrl"), m_baseTextureUrl);
	_jsonObj->TryGetStringField(TEXT("meshNormalUrl"), m_meshNormalUrl);
	_jsonObj->TryGetStringField(TEXT("iconUrl"), m_IconUrl);
	_jsonObj->TryGetBoolField(TEXT("isRib"), m_isRib);
}
