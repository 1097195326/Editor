// Fill out your copyright notice in the Description page of Project Settings.

#include "StylePartR.h"

StylePartR::StylePartR()
{
}

StylePartR::~StylePartR()
{
}

void StylePartR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("suitPartId"));

	m_styleComponentId = _jsonObj->GetIntegerField(TEXT("styleComponentId"));
}

void StylePartR::OnLoad()
{

}

void StylePartR::OnRelease()
{

}
