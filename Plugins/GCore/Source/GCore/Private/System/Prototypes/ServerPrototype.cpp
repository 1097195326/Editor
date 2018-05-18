// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "ServerPrototype.h"

GREGISTER(ServerPrototype)
ServerPrototype::ServerPrototype()
{
}

ServerPrototype::~ServerPrototype()
{
}

void ServerPrototype::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("key");

		const char * mValue = elem->Attribute("value");

		m_values.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mValue));
	}
}

void ServerPrototype::OnLoad()
{

}

void ServerPrototype::OnRelease()
{
	m_values.Empty();
}

const FString ServerPrototype::GetValueByKey(const FString & _key)
{
	if (m_values.Contains(_key))
	{
		return m_values[_key];
	}
	return TEXT("");
}
 