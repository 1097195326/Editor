// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleSliceC.h"
#include "RStyleSliceManager.h"
#include "StyleSliceR.h"
#include "SuitSliceT.h"
#include "RStylePatternManager.h"
#include "RStyleFabricManager.h"
#include "SuitFabricLibraryT.h"
#include "SuitFabricColorLibraryT.h"
#include "TSuitFabricColorLibraryManager.h"
#include "TSuitFabricLibraryManager.h"
#include "SuitFabricT.h"
#include "TSuitFabricManager.h"
#include "TSuitSliceManager.h"

StyleSliceC::StyleSliceC()
{
}

StyleSliceC::~StyleSliceC()
{
}

void StyleSliceC::OnLoad()
{

}

void StyleSliceC::OnRelease()
{

}

void StyleSliceC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	BaseR * m_dataR = RStyleSliceManager::GetInstance()->GetRuntimeByPID(_rid);

	m_dataT = TSuitSliceManager::GetInstance()->GetTemplateByPID(_tid);

	// 创建C数据.
	if (nullptr != m_dataR)
	{
		StyleSliceR * mFabricR = (StyleSliceR *)m_dataR;

		m_suitFabricId = mFabricR->m_suitFabricId;

		m_suitPatternId = mFabricR->m_suitPatternId;

		m_suitColorId = mFabricR->m_suitColorId;

		m_angle = mFabricR->m_angle;

		m_offset = mFabricR->m_offset;

		m_uVScaleUser = mFabricR->m_uVScaleUser;
	}
	else
	{
		SuitSliceT * mSliceT = (SuitSliceT *)m_dataT;

		int32 mFabricLibId = mSliceT->m_FabricLibId;

		SuitFabricLibraryT * mFabricLib =TSuitFabricLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricLibraryT>(mFabricLibId);

		TArray<int32> mFabricArray = mFabricLib->m_Fabrics;

		if (mFabricArray.Num() > 0)
			m_suitFabricId = mFabricArray[0];

		// 花型优先.
		SuitFabricT * mFabricT = TSuitFabricManager::GetInstance()->GetTemplateByPID<SuitFabricT>(m_suitFabricId);

		if (nullptr != mFabricT)
		{
			int32 mFabricColorId = mFabricT->m_ColorRepositoryId;

			SuitFabricColorLibraryT * mFabricColorLib = TSuitFabricColorLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricColorLibraryT>(mFabricColorId);

			if (mFabricColorLib)
			{
				if (mFabricColorLib->m_fabricColorIds.Num() > 0)
					m_suitColorId = mFabricColorLib->m_fabricColorIds[0];
			}
		}
	}
}

void StyleSliceC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}
