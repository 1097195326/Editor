// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "SchedulerMapping.h"

GREGISTER(SchedulerMapping)

SchedulerMapping::SchedulerMapping()
{
}

SchedulerMapping::~SchedulerMapping()
{
}

void SchedulerMapping::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("key");

		const char * mValue = elem->Attribute("value");

		m_values.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mValue));
	}
}

void SchedulerMapping::OnLoad()
{

}

void SchedulerMapping::OnRelease()
{

}

FString SchedulerMapping::GetClassName(const FString & _className)
{
	if (m_values.Contains(_className))
	{
		return m_values[_className];
	}
	return TEXT("");
}
