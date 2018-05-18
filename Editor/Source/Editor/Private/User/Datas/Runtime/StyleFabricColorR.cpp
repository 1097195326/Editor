// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleFabricColorR.h"

StyleFabricColorR::StyleFabricColorR()
{
}

StyleFabricColorR::~StyleFabricColorR()
{
}

void StyleFabricColorR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("styleFabricColorId"));

}

void StyleFabricColorR::OnLoad()
{

}

void StyleFabricColorR::OnRelease()
{

}
