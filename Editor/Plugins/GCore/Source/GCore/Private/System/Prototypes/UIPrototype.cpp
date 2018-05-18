// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "UIPrototype.h"
GREGISTER(UIPrototype)
UIPrototype::UIPrototype()
{
}

UIPrototype::~UIPrototype()
{
}

void UIPrototype::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("key");

		const char * mValue = elem->Attribute("value");

		const char * mWidth = elem->Attribute("width");

		const char * mHeight = elem->Attribute("height");

		m_screenWidth = atoi(mWidth);

		m_scrrenHeight = atoi(mHeight);

		m_values.Add(UTF8_TO_TCHAR(mKey), UTF8_TO_TCHAR(mValue));
	}
}

void UIPrototype::OnLoad()
{

}

void UIPrototype::OnRelease()
{
	m_values.Empty();
}

int32 UIPrototype::GetUIPrototypeId(const FString & _key)
{
	FString mDefaultKey = TEXT("0.56");
	float mKey = FCString::Atof(*_key);
	float mPresentKey = 300.0f;
	for (auto Iter = m_values.CreateIterator(); Iter; ++Iter)
	{
		float mCKey = FCString::Atof(*Iter->Key);
		FString mValue = Iter->Value;
		if (mKey == mCKey)
		{
			return FCString::Atoi(*mValue);
		}
		else if (mPresentKey<mKey && mKey<mCKey)
		{
			return FCString::Atoi(*mValue);  
		}

		mPresentKey = mCKey;
	}
	// 要是没有就选中默认的UI显示.
	return FCString::Atoi(*m_values[*mDefaultKey]);
}
