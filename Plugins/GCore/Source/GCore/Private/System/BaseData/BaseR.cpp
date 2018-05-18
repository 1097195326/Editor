// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "BaseR.h"




int32 BaseR::GetTid()
{
	return m_Tid;
}

int32 BaseR::GetRid()
{
	return m_Rid;
}

BaseR::BaseR()
{

}

BaseR::~BaseR()
{

}

void BaseR::GLoad()
{

	OnLoad();
}

void BaseR::GRelease()
{
	OnRelease();

	delete this;
}

void BaseR::InitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	// 先解析父类需要的变量.
	m_Rid = _jsonObj->GetIntegerField("id");

	m_name = _jsonObj->GetStringField("name");

	OnInitWithJson(_jsonObj);
}

void BaseR::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{

}

