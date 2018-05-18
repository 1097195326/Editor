// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleFabricR.h"

StyleFabricR::StyleFabricR()
{
}

StyleFabricR::~StyleFabricR()
{
}

void StyleFabricR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("suitFabricId"));

	m_styleColorId = _jsonObj->GetIntegerField(TEXT("styleColorId"));

}

void StyleFabricR::OnLoad()
{

}

void StyleFabricR::OnRelease()
{

}
