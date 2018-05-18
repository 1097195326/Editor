   // Fill out your copyright notice in the Description page of Project Settings.

#include "StylePatternR.h"

StylePatternR::StylePatternR()
{
}

StylePatternR::~StylePatternR()
{
}

void StylePatternR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid = _jsonObj->GetIntegerField(TEXT("suitPatternId"));


}

void StylePatternR::OnLoad()
{

}

void StylePatternR::OnRelease()
{

}
