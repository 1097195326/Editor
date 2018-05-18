// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "BasePrototype.h"

BasePrototype::BasePrototype()
{
}

BasePrototype::~BasePrototype()
{
}

void BasePrototype::LoadXMLData(TiXmlElement * _elem)
{
	const char * mPid = _elem->Attribute("pid");

	const char * mName = _elem->Attribute("name");

	const char * mItemDescript = _elem->Attribute("itemDescript");

	m_pid = atoi(mPid);

	m_name = mName != 0 ? UTF8_TO_TCHAR(mName) : UTF8_TO_TCHAR("");

	OnLoadXMLData(_elem);
}


void BasePrototype::OnLoadXMLData(TiXmlElement * _elem)
{

}


void BasePrototype::OnRelease()
{

}

void BasePrototype::OnLoad()
{

}

void BasePrototype::GLoad()
{
	OnLoad();
}

void BasePrototype::GRelease()
{
	OnRelease();

	delete this;
}
