// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "LanguagePrototype.h"

GREGISTER(LanguagePrototype)
LanguagePrototype::LanguagePrototype()
{
}

LanguagePrototype::~LanguagePrototype()
{
}

void LanguagePrototype::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("key");

		const char * mValue = elem->Attribute("value");

		const char * mAssetUrl = elem->Attribute("asset");

		m_values.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mValue));

		m_assetValues.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mAssetUrl));
	}
}

void LanguagePrototype::OnLoad()
{

}

void LanguagePrototype::OnRelease()
{
	m_values.Empty();

	m_assetValues.Empty();
}

FString LanguagePrototype::GetLanguage(const FString & _key)
{
	if (m_values.Contains(_key))
	{
		return m_values[_key];
	}
	return TEXT("");
}

FString LanguagePrototype::GetLanguageTexture2D(const FString & _key)
{
	if (m_assetValues.Contains(_key))
	{
		return m_assetValues[_key];
	}
	return TEXT("");
}
