// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitPatternT.h"

SuitPatternT::SuitPatternT()
{
}

SuitPatternT::~SuitPatternT()
{
}

void SuitPatternT::OnLoad()
{

}

void SuitPatternT::OnRelease()
{

}

void SuitPatternT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_buy=_jsonObj->GetBoolField(TEXT("buy"));
	m_height=_jsonObj->GetIntegerField(TEXT("height"));
	m_iconUrl=_jsonObj->GetStringField(TEXT("iconUrl"));
	m_ownerId=_jsonObj->GetIntegerField(TEXT("ownerId"));
	m_ownerType = _jsonObj->GetStringField(TEXT("ownerType"));
	m_Sn = _jsonObj->GetStringField(TEXT("sn"));
	m_SupplierId=_jsonObj->GetIntegerField(TEXT("supplierId"));
	m_SystemId=_jsonObj->GetStringField(TEXT("systemId"));
	m_Width=_jsonObj->GetIntegerField(TEXT("width"));
	_jsonObj->TryGetStringField(TEXT("baseTextureUrl"), m_baseTextureUrl);
	// 获取主键.
	_jsonObj->TryGetStringField(TEXT("patternId"), m_patternId);
}
