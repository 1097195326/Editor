// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitFabricT.h"

SuitFabricT::SuitFabricT()
{
}

SuitFabricT::~SuitFabricT()
{
}

void SuitFabricT::OnLoad()
{

}

void SuitFabricT::OnRelease()
{

}

void SuitFabricT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_BasePatternUrl=_jsonObj->GetStringField(TEXT("basePatternUrl"));
	m_CanPattern=_jsonObj->GetBoolField(TEXT("canPattern"));
	m_CanShowDetails = _jsonObj->GetBoolField(TEXT("canShowDetails"));
	_jsonObj->TryGetNumberField(TEXT("colorRepositoryId"),m_ColorRepositoryId);
	m_IconUrl = _jsonObj->GetStringField(TEXT("iconUrl"));

	m_MaskUrl = _jsonObj->GetStringField(TEXT("maskUrl"));
	m_MaterialTemplateUrl = _jsonObj->GetStringField(TEXT("materialTemplateUrl"));
	m_MetalUrl = _jsonObj->GetStringField(TEXT("metalUrl"));

	m_normalOriginalSize = _jsonObj->GetNumberField(TEXT("normalOriginalSize"));

	m_NormalUrl = _jsonObj->GetStringField(TEXT("normalUrl"));
	m_PrimaryPantoneCode = _jsonObj->GetStringField(TEXT("primaryPantoneCode"));
	m_RoughnessUrl = _jsonObj->GetStringField(TEXT("roughnessUrl"));
	//!!！询问传的是不是String.
	const TSharedPtr<FJsonObject>& mObj =_jsonObj->GetObjectField(TEXT("uVScale"));
	m_UVScale.X = mObj->GetNumberField(TEXT("x"));
	m_UVScale.Y = mObj->GetNumberField(TEXT("y"));

}
