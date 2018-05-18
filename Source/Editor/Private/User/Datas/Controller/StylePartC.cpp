// Fill out your copyright notice in the Description page of Project Settings.

#include "StylePartC.h"
#include "RStylePartManager.h"
#include "TSuitPartManager.h"
#include "RStylePartManager.h"
#include "RStyleComponentManager.h"
#include "StylePartR.h"
#include "SuitPartT.h"

StylePartC::StylePartC()
{
}

StylePartC::~StylePartC()
{
}

void StylePartC::OnLoad()
{

}

void StylePartC::OnRelease()
{

}

void StylePartC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	BaseR * m_dataR = RStylePartManager::GetInstance()->GetRuntimeByPID(_rid);

	m_dataT = TSuitPartManager::GetInstance()->GetTemplateByPID(_tid);

	// 创建C数据.
	if (nullptr != m_dataR)
	{
		StylePartR * mFabricR = (StylePartR *)m_dataR;

		m_suitComponentId = RStyleComponentManager::GetInstance()->GetRuntimeByPID(mFabricR->m_styleComponentId)->GetTid();

		m_styleComponentId = mFabricR->m_styleComponentId; 
	}
	else
	{
		SuitPartT * mSuitPartT = (SuitPartT *)m_dataT;

		m_suitComponentId = mSuitPartT->m_SuitComponentIds[0];
	}
}

void StylePartC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}
