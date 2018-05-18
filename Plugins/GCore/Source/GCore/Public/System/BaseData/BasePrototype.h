// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "castle_std.h"
#include "ILoad.h"
#include "GObject.h"
#include "GCore/Public/AAReflection/AARefClasses.h"
/**
 * 
 */
class GCORE_API BasePrototype :public GObject, public ILoad
{
public:
	BasePrototype();

	virtual ~BasePrototype();

protected:

	int32	    	m_pid;

	FString			m_name;

public:

	PropertyGetByName(int32, pid, public)

	PropertyGetByName(FString, name, public)

protected:

	virtual void OnLoadXMLData(TiXmlElement * _elem);

	virtual void OnLoad();

	virtual void OnRelease();


	// 功能方法.
public:

	virtual void GLoad() final;

	virtual void GRelease() final;

	virtual void LoadXMLData(TiXmlElement * _elem) final;
	
};
