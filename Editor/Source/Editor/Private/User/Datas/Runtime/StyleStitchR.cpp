  // Fill out your copyright notice in the Description page of Project Settings.

#include "StyleStitchR.h"

StyleStitchR::StyleStitchR()
{
}

StyleStitchR::~StyleStitchR()
{
}

void StyleStitchR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("suitStitchPartId"));

	m_styleComponentId = _jsonObj->GetNumberField(TEXT("styleStitchComponentId"));
}

void StyleStitchR::OnLoad()
{

}

void StyleStitchR::OnRelease()
{

}
