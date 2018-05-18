// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "GLanguageTool.h"
#include "GameDirector.h"
#include "PrototypeManager.h"
#include "LanguagePrototype.h"

GLanguageTool* GLanguageTool::m_pInstance = nullptr;//初始化在主线程之前.

GLanguageTool::GLanguageTool()
{
}

GLanguageTool::~GLanguageTool()
{
}

GLanguageTool * GLanguageTool::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new GLanguageTool();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

void GLanguageTool::GLoad()
{
	UGameDirector::GetInstance()->RegisterInstanceClass(this);
}

void GLanguageTool::GRelease()
{
	m_pInstance = nullptr;

	delete this;
}

FString GLanguageTool::GetLanguage(const FString & _key)
{
	return PrototypeManager::GetInstance()->GetPrototypeByPID<LanguagePrototype>(PrototypeConst::P_LANGUAGE_ID)->GetLanguage(_key);
}


FString GLanguageTool::GetLanguageTexture2D(const FString & _key)
{
	return PrototypeManager::GetInstance()->GetPrototypeByPID<LanguagePrototype>(PrototypeConst::P_LANGUAGE_ID)->GetLanguage(_key);
}
