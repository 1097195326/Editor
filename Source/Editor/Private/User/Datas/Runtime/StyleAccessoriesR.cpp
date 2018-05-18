// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleAccessoriesR.h"

StyleAccessoriesR::StyleAccessoriesR()
{
}

StyleAccessoriesR::~StyleAccessoriesR()
{
}

void StyleAccessoriesR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("suitAccessoryPartId"));

	m_styleComponentId = _jsonObj->GetIntegerField(TEXT("styleAccessoryComponentId"));
}

void StyleAccessoriesR::OnLoad()
{

}

void StyleAccessoriesR::OnRelease()
{

}
