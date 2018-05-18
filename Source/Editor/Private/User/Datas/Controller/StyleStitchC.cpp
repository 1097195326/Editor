// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleStitchC.h"
#include "RStyleStitchManager.h"
#include "TSuitStitchManager.h"
#include "StyleStitchR.h"
#include "RStyleComponentManager.h"
#include "SuitStitchT.h"

StyleStitchC::StyleStitchC()
{
}

StyleStitchC::~StyleStitchC()
{
}

void StyleStitchC::OnLoad()
{

}

void StyleStitchC::OnRelease()
{

}

void StyleStitchC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	BaseR * m_dataR = RStyleStitchManager::GetInstance()->GetRuntimeByPID(_rid);

	m_dataT = TSuitStitchManager::GetInstance()->GetTemplateByPID(_tid);

	if (nullptr == m_dataT)
		return;

	// 创建C数据.
	if (nullptr != m_dataR)
	{
		StyleStitchR * mFabricR = (StyleStitchR *)m_dataR;

		m_StyleComponentId = mFabricR->m_styleComponentId;

		m_SuitComponentId = RStyleComponentManager::GetInstance()->GetRuntimeByPID(m_StyleComponentId)->GetTid();
	}
	else
	{
		SuitStitchT * mStitchT = (SuitStitchT*)m_dataT;

		TArray<int32> mStitchArrary = mStitchT->m_SuitComponentIds;

		if (mStitchArrary.Num()>0)
		{
			m_SuitComponentId = mStitchArrary[0];
		}
	}
}

void StyleStitchC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}
