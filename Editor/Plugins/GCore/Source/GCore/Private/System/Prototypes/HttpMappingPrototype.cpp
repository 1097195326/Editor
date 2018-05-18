// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "HttpMappingPrototype.h"

GREGISTER(HttpMappingPrototype)

HttpMappingPrototype::HttpMappingPrototype()
{
}

HttpMappingPrototype::~HttpMappingPrototype()
{
}


void HttpMappingPrototype::OnLoadXMLData(TiXmlElement * _elem)
{
	for (TiXmlElement* elem = _elem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		const char * mKey = elem->Attribute("msgId");

		const char * mValue = elem->Attribute("channel");

		const char * mUrl = elem->Attribute("url");

		if (mKey != nullptr)
		{
			int32 mMsgId = atoi(mKey);

			FString mChannelName = UTF8_TO_TCHAR(mValue);

			FString mHttpUrl = UTF8_TO_TCHAR(mUrl);

			HttpMapping * mMapping = new HttpMapping();

			mMapping->m_httpUri = mHttpUrl;

			mMapping->m_channelName = mChannelName;

			// 直接注册到GHttpManager里.
			m_mapping.Add(mMsgId, mMapping);
		}
	}
}

HttpMapping * HttpMappingPrototype::GetMappingByMsgID(int32 _msgHttpId)
{
	if (!m_mapping.Contains(_msgHttpId))
	{
		return nullptr;
	}

	return m_mapping[_msgHttpId];
}

void HttpMappingPrototype::OnLoad()
{

}

void HttpMappingPrototype::OnRelease()
{
	for (auto& Elem : m_mapping)
	{
		HttpMapping * mMapping = Elem.Value;

		delete mMapping;
	}

	m_mapping.Empty();
}