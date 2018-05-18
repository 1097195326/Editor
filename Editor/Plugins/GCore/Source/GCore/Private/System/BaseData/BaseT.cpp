// Fill out your copyright notice in the Description page of Project Settings.
#include "GCore.h"
#include "BaseT.h"




BaseT::BaseT()
{}

BaseT::~BaseT()
{}

int32 BaseT::GetTid()
{
	return m_Tid;
}

void BaseT::GLoad()
{
	OnLoad();
}

void BaseT::GRelease()
{
	OnRelease();

	delete this;
}

void BaseT::InitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	// 先解析父类需要的变量.
	m_Tid = _jsonObj->GetIntegerField("id");

	m_name = _jsonObj->GetStringField("name");

	OnInitWithJson(_jsonObj);
}

void BaseT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{

}
