// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleC.h"
#include "RSysStyleManager.h"
#include "RUserStyleManager.h"
#include "TSuitManager.h"
#include "StyleR.h"
#include "DesignModule.h"
#include "RStylePartManager.h"
#include "GModuleManager.h"


StyleC::StyleC()
{
}

StyleC::~StyleC()
{
}

void StyleC::OnLoad()
{

}

void StyleC::OnRelease()
{
	for (int32 i = m_PartIdArray.Num() - 1; i >= 0; i--)
	{
		ChildPartStruct * mStruct = m_PartIdArray[i];

		delete mStruct;
	}
	m_PartIdArray.Empty();
}

void StyleC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	BaseR * m_dataR = nullptr;

	if (mDesignModule->GetDesignData()->m_DesignType == EnumDesignType::DESIGN_BY_SYS_STYLE)
	{
		m_dataR = RSysStyleManager::GetInstance()->GetRuntimeByPID(_rid);

		m_isRecommend = 0;
	}
	else if (mDesignModule->GetDesignData()->m_DesignType == EnumDesignType::DESIGN_BY_USER_STYLE)
	{
		m_dataR = RUserStyleManager::GetInstance()->GetRuntimeByPID(_rid);

		m_isRecommend = 0;
	}
	else
	{
		m_isRecommend = 1;
	}

	m_dataT = TSuitManager::GetInstance()->GetTemplateByPID(_tid);

	if (nullptr != m_dataR)
	{
		// 创建C数据.
		StyleR * mStyleR = (StyleR *)m_dataR;

		m_iconUrl = mStyleR->m_iconUrl;

		m_commont = mStyleR->m_commont;

		TArray<int32> mPartArray = mStyleR->m_stylePartIdArray;

		TArray<int32> mSuitPartIdArray = ((SuitT*)m_dataT)->m_ArrayPartId;

		for (int32 i = 0; i < mSuitPartIdArray.Num(); i++)
		{
			int32 mSuitTid= mSuitPartIdArray[i];

			bool isExits = false;

			for (int32 j = 0; j < mPartArray.Num(); j++)
			{
				int32 mSuitPartId = RStylePartManager::GetInstance()->GetRuntimeByPID(mPartArray[j])->GetTid();

				if (mSuitTid == mSuitPartId)
				{
					isExits = true;

					ChildPartStruct * mPartStruct = new ChildPartStruct();

					mPartStruct->m_stylePartId = mPartArray[j];

					mPartStruct->m_suitPartId = mSuitPartId;

					m_PartIdArray.Add(mPartStruct);

					break;
				}
			}

			if (!isExits)
			{
				ChildPartStruct * mPartStruct = new ChildPartStruct();

				mPartStruct->m_suitPartId = mSuitTid;

				m_PartIdArray.Add(mPartStruct);
			}
		}
	}
	else
	{
		TArray<int32> mSuitPartIdArray = ((SuitT*)m_dataT)->m_ArrayPartId;

		for (int32 i=0;i<mSuitPartIdArray.Num();i++)
		{
			ChildPartStruct * mPartStruct = new ChildPartStruct();

			mPartStruct->m_suitPartId = mSuitPartIdArray[i];

			m_PartIdArray.Add(mPartStruct);
		}
	}
}

void StyleC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}
