// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "ScenePrototype.h"

GREGISTER(ScenePrototype)

ScenePrototype::ScenePrototype()
{
}

ScenePrototype::~ScenePrototype()
{
}

void ScenePrototype::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("key");

		const char * mValue = elem->Attribute("value");

		m_values.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mValue));
	}
}

void ScenePrototype::OnLoad()
{

}

void ScenePrototype::OnRelease()
{
	m_values.Empty();
}

FString ScenePrototype::GetLevelName(const FString & _levelName)
{
	if (m_values.Contains(_levelName))
	{
		return m_values[_levelName];
	}
	return TEXT("");
}
