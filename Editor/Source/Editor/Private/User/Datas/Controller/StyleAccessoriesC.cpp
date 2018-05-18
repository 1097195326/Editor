// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleAccessoriesC.h"
#include "RStyleAccessoriesManager.h"
#include "TSuitAccessoriesManager.h"
#include "StyleAccessoriesR.h"
#include "SuitAccessoriesT.h"
#include "StyleComponentR.h"
#include "RStyleComponentManager.h"

StyleAccessoriesC::StyleAccessoriesC()
{
}

StyleAccessoriesC::~StyleAccessoriesC()
{
}

void StyleAccessoriesC::OnLoad()
{

}

void StyleAccessoriesC::OnRelease()
{

}

void StyleAccessoriesC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	BaseR* m_dataR = RStyleAccessoriesManager::GetInstance()->GetRuntimeByPID(_rid);

	m_dataT = TSuitAccessoriesManager::GetInstance()->GetTemplateByPID(_tid);

	// 创建C数据.
	if (nullptr != m_dataR)
	{
		StyleAccessoriesR * mAccessoriesR = (StyleAccessoriesR *)m_dataR;

		m_styleComponentId = mAccessoriesR->m_styleComponentId;

		StyleComponentR * mStyleComponentR = RStyleComponentManager::GetInstance()->GetRuntimeByPID<StyleComponentR>(m_styleComponentId);

		m_suitComponentId = mStyleComponentR->GetTid();
	}
	else
	{
		SuitAccessoriesT * mAccessoriesT = (SuitAccessoriesT *)m_dataT;

		TArray<int32> mSuitComponentIds = mAccessoriesT->m_SuitComponent;

		if (mSuitComponentIds.Num() > 0)
		{
			m_suitComponentId = mSuitComponentIds[0];
		}
	}
}

void StyleAccessoriesC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}
