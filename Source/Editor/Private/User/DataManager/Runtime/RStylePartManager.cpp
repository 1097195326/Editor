// Fill out your copyright notice in the Description page of Project Settings.

#include "RStylePartManager.h"
#include "RSysStyleManager.h"
#include "StyleR.h"
#include "StylePartR.h"
#include "RUserStyleManager.h"

RStylePartManager *   RStylePartManager::m_pInstance = nullptr;

RStylePartManager * RStylePartManager::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = new RStylePartManager();

		m_pInstance->GLoad();
	}
	return m_pInstance;
}

RStylePartManager::RStylePartManager()
{
}

RStylePartManager::~RStylePartManager()
{
}

void RStylePartManager::OnLoad()
{

}

void RStylePartManager::OnRelease()
{
	m_pInstance = nullptr;
}

void RStylePartManager::EncodeData(TSharedPtr<FJsonObject> & _content)
{
	// 反序列化数据并添加到管理器中.
	TArray<TSharedPtr<FJsonValue>> mStylePart = _content->GetArrayField("stylePart");

	int32 mStyleId = _content->GetIntegerField("styleId");

	for (int32 i = 0; i < mStylePart.Num(); i++)
	{
		TSharedPtr<FJsonObject> mContentJson = mStylePart[i]->AsObject();

		StylePartR * mStylePart = new StylePartR();

		mStylePart->GLoad();

		mStylePart->InitWithJson(mContentJson);

		AddData(mStylePart);
	}
}

