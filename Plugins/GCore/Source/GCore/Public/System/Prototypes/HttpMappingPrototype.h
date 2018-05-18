// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "castle_std.h"
#include "BasePrototype.h"

class HttpMapping
{
public:

	FString m_httpUri;

	FString m_channelName;
};

/**
*
*/
class GCORE_API HttpMappingPrototype :public BasePrototype
{
private:
	TMap<int32, HttpMapping*>	    m_mapping;

public:
	HttpMappingPrototype();
	virtual ~HttpMappingPrototype();

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem) override;

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:
	HttpMapping * GetMappingByMsgID(int32 _msgHttpId);
};
