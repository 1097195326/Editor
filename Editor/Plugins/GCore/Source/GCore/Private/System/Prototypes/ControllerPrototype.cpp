// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "ControllerPrototype.h"
#include "castle_std.h"
#include "GController.h"

GREGISTER(ControllerPrototype)

ControllerPrototype::ControllerPrototype()
{
}

ControllerPrototype::~ControllerPrototype()
{
}

void ControllerPrototype::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("key");

		const char * mValue = elem->Attribute("value");

		m_values.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mValue));
	}

	// 把设置内容放在GController类里面.
	GController::m_isDebug = this->IsDebug();

	GController::m_language = this->GetLanguage();

	GController::m_platform = this->GetPlatform();

	GController::m_appVersion = this->GetAppVersion();

	GController::m_isadaptation = IsAdaptation();

	GController::m_isAdmin = IsAdmin();
}

void ControllerPrototype::OnLoad()
{

}

void ControllerPrototype::OnRelease()
{
	m_values.Empty();
}

const FString ControllerPrototype::GetValueByKey(const FString & _key)
{
	if (m_values.Contains(_key))
	{
		return m_values[_key];
	}
	return TEXT("");
}

bool ControllerPrototype::IsDebug()
{
	FString mKey = TEXT("isDebug");

	if (m_values.Contains(mKey))
	{
		return m_values[mKey] == TEXT("1") ? true : false;
	}
	return false;
}

bool ControllerPrototype::IsAdmin()
{
	FString mKey = TEXT("isAdmin");

	if (m_values.Contains(mKey))
	{
		return m_values[mKey] == TEXT("1") ? true : false;
	}
	return false;
}

bool ControllerPrototype::IsAdaptation()
{
	FString mKey = TEXT("adaptation");

	if (m_values.Contains(mKey))
	{
		return m_values[mKey] == TEXT("1") ? true : false;
	}
	return false;
}

// 获取平台.
ENUM_PLATFORM ControllerPrototype::GetPlatform()
{
	FString mKey = TEXT("platform");

	if (m_values.Contains(mKey))
	{
		return ENUM_PLATFORM(atoi(TCHAR_TO_UTF8(*(m_values[mKey]))));
	}
	return ENUM_PLATFORM::GWINDOWS;
}

// 获取要运行的语言.
ENUM_LANGUAGE ControllerPrototype::GetLanguage()
{
	FString mKey = TEXT("language");

	if (m_values.Contains(mKey))
	{
		return ENUM_LANGUAGE(atoi(TCHAR_TO_UTF8(*(m_values[mKey]))));
	}
	return ENUM_LANGUAGE::GCHINESE;
}

FString ControllerPrototype::GetAppVersion()
{
	FString mKey = TEXT("appVersion");

	if (m_values.Contains(mKey))
	{
		return m_values[mKey];
	}
	return TEXT("");
}
