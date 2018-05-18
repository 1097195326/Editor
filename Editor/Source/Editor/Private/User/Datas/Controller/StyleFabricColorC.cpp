// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleFabricColorC.h"
#include "RStyleFabricColorManager.h"
#include "StyleFabricColorR.h"
#include "TSuitFabricColorManager.h"

StyleFabricColorC::StyleFabricColorC()
{
}

StyleFabricColorC::~StyleFabricColorC()
{
}

void StyleFabricColorC::OnLoad()
{

}

void StyleFabricColorC::OnRelease()
{

}

void StyleFabricColorC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	m_dataT = TSuitFabricColorManager::GetInstance()->GetTemplateByPID(_tid);
}

void StyleFabricColorC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}
