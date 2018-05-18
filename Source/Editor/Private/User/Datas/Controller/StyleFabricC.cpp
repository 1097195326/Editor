// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleFabricC.h"
#include "RStyleFabricManager.h"
#include "StyleFabricR.h"
#include "SuitFabricT.h"
#include "RStyleFabricColorManager.h"
#include "TSuitFabricManager.h"

StyleFabricC::StyleFabricC()
{
}

StyleFabricC::~StyleFabricC()
{
}

void StyleFabricC::OnLoad()
{

}

void StyleFabricC::OnRelease()
{

}

void StyleFabricC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	m_dataT = TSuitFabricManager::GetInstance()->GetTemplateByPID(_tid);
}

void StyleFabricC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}
