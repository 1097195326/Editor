// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "PlatformUIPrototype.h"

GREGISTER(PlatformUIPrototype)

PlatformUIPrototype::PlatformUIPrototype()
{
}

PlatformUIPrototype::~PlatformUIPrototype()
{
}

void PlatformUIPrototype::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("key");

		const char * mValue = elem->Attribute("value");

		const char * mParentUI = elem->Attribute("parentUI");

		m_values.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mValue));

		m_parentUI.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mParentUI));
	}
}

void PlatformUIPrototype::OnLoad()
{

}

void PlatformUIPrototype::OnRelease()
{
	m_values.Empty();
}

FString PlatformUIPrototype::GetUMGPath(const FString & _key)
{
	if (m_values.Contains(_key))
	{
		return m_values[_key];
	}
	return TEXT("");
}

FString PlatformUIPrototype::GetParentUIName(const FString & _key)
{
	if (m_parentUI.Contains(_key))
	{
		return m_parentUI[_key];
	}
	return TEXT("");
}
