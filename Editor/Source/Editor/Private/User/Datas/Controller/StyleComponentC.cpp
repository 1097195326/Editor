// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleComponentC.h"
#include "RStyleComponentManager.h"
#include "TSuitComponentManager.h"
#include "StyleComponentR.h"
#include "RStyleSliceManager.h"
#include "SuitComponentT.h"
#include "RStyleStitchManager.h"
#include "RStyleAccessoriesManager.h"

StyleComponentC::StyleComponentC()
{
}

StyleComponentC::~StyleComponentC()
{
}

void StyleComponentC::OnLoad()
{

}

void StyleComponentC::OnRelease()
{
	for (int32 i = m_AccessoryIds.Num() - 1; i >= 0; i--)
	{
		delete m_AccessoryIds[i];
	}

	m_AccessoryIds.Empty();

	for (int32 i = m_SliceIds.Num() - 1; i >= 0; i--)
	{
		delete m_SliceIds[i];
	}

	m_SliceIds.Empty();

	for (int32 i = m_StitchsuitIds.Num() - 1; i >= 0; i--)
	{
		delete m_StitchsuitIds[i];
	}

	m_StitchsuitIds.Empty();
}

void StyleComponentC::OnInitData(int32 _cid, int32 _rid, int32 _tid)
{
	CreateAccessory(_rid, _tid);
	CreateSlice(_rid, _tid);
	CreateStitch(_rid,_tid);
}

void StyleComponentC::AddJsonC(TSharedPtr<FJsonObject> & _jsonObj)
{

}

void StyleComponentC::CreateAccessory(int32 _rid, int32 _tid)
{
	BaseR * m_dataR = RStyleComponentManager::GetInstance()->GetRuntimeByPID(_rid);

	m_dataT = TSuitComponentManager::GetInstance()->GetTemplateByPID(_tid);

	SuitComponentT * mSuitComponentT = (SuitComponentT *)m_dataT;

	TArray<int32> mSuitAccessoryIds = mSuitComponentT->m_SuitAccessoriesIds;

	// 创建C数据.
	if (nullptr != m_dataR)
	{
		StyleComponentR * mDataR = (StyleComponentR *)m_dataR;

		TArray<int32> mAccessoryIds = mDataR->m_styleAccessoryIds;

		for (int32 j = 0; j < mSuitAccessoryIds.Num(); j++)
		{
			int32 mSuitAccessoryId = mSuitAccessoryIds[j];

			bool isExits = false;

			for (int32 i = 0; i < mAccessoryIds.Num(); i++)
			{
				int32 mTid = RStyleAccessoriesManager::GetInstance()->GetRuntimeByPID(mAccessoryIds[i])->GetTid();
				
				if (mTid == mSuitAccessoryId)
				{
					isExits = true;

					CAccessoryStruct * mStruct = new CAccessoryStruct();

					mStruct->m_styleAccessoryId = mAccessoryIds[i];

					mStruct->m_suitAccessoryId = mTid;

					m_AccessoryIds.Add(mStruct);

					break;
				}
			}

			if (!isExits)
			{
				CAccessoryStruct * mStruct = new CAccessoryStruct();

				mStruct->m_suitAccessoryId = mSuitAccessoryIds[j];

				m_AccessoryIds.Add(mStruct);
			}
		}
	}
	else
	{
		for (int32 j = 0; j < mSuitAccessoryIds.Num(); j++)
		{
			CAccessoryStruct * mStruct = new CAccessoryStruct();

			mStruct->m_suitAccessoryId = mSuitAccessoryIds[j];

			m_AccessoryIds.Add(mStruct);
		}
	}
}

void StyleComponentC::CreateSlice(int32 _rid, int32 _tid)
{
	BaseR * m_dataR = RStyleComponentManager::GetInstance()->GetRuntimeByPID(_rid);

	m_dataT = TSuitComponentManager::GetInstance()->GetTemplateByPID(_tid);

	SuitComponentT * mSuitComponentT = (SuitComponentT *)m_dataT;

	TArray<int32> mSuitSliceIds = mSuitComponentT->m_SuitSliceIds;

	if (nullptr != m_dataR)
	{
		StyleComponentR * mDataR = (StyleComponentR *)m_dataR;

		TArray<int32> mSliceIds = mDataR->m_styleSliceIds;

		for (int32 j = 0; j < mSuitSliceIds.Num(); j++)
		{
			int32 mSuitSliceId = mSuitSliceIds[j];

			bool isExist = false;

			for (int32 i = 0; i < mSliceIds.Num(); i++)
			{
				int32 mTid = RStyleSliceManager::GetInstance()->GetRuntimeByPID(mSliceIds[i])->GetTid();

				if (mSuitSliceId == mTid)
				{
					isExist = true;

					SliceStruct * mStruct = new SliceStruct();

					mStruct->m_styleSliceId = mSliceIds[i];

					mStruct->m_suitSliceId = mTid;

					m_SliceIds.Add(mStruct);

					break;
				}
			}

			if (!isExist)
			{
				SliceStruct * mStruct = new SliceStruct();

				mStruct->m_suitSliceId = mSuitSliceIds[j];

				m_SliceIds.Add(mStruct);
			}
		}
	}
	else
	{
		for (int32 j = 0; j < mSuitSliceIds.Num(); j++)
		{
			SliceStruct * mStruct = new SliceStruct();

			mStruct->m_suitSliceId = mSuitSliceIds[j];

			m_SliceIds.Add(mStruct);
		}
	}
}

void StyleComponentC::CreateStitch(int32 _rid, int32 _tid)
{
	BaseR * m_dataR = RStyleComponentManager::GetInstance()->GetRuntimeByPID(_rid);

	m_dataT = TSuitComponentManager::GetInstance()->GetTemplateByPID(_tid);

	SuitComponentT * mSuitComponentT = (SuitComponentT *)m_dataT;

	TArray<int32> mSuitStitchIds = mSuitComponentT->m_SuitStitchsuitIds;

	if (nullptr != m_dataR)
	{
		StyleComponentR * mDataR = (StyleComponentR *)m_dataR;

		TArray<int32> mStitchIds = mDataR->m_styleStitchsuitIds;

		for (int32 j = 0; j < mSuitStitchIds.Num(); j++)
		{
			int32 mStitchTid = mSuitStitchIds[j];

			bool isExits = false;

			for (int32 i = 0; i < mStitchIds.Num(); i++)
			{
				int32 mTid = RStyleStitchManager::GetInstance()->GetRuntimeByPID(mStitchIds[i])->GetTid();

				if (mTid == mStitchTid)
				{
					isExits = true;

					StitchStruct * mStruct = new StitchStruct();

					mStruct->m_styleStitchId = mStitchIds[i];

					mStruct->m_suitStitchId = mTid;

					m_StitchsuitIds.Add(mStruct);

					break;
				}
			}
			if (!isExits)
			{
				StitchStruct * mStruct = new StitchStruct();

				mStruct->m_suitStitchId = mSuitStitchIds[j];

				m_StitchsuitIds.Add(mStruct);	
			}
		}
	}
	else
	{
		for (int32 j = 0; j < mSuitStitchIds.Num(); j++)
		{
			StitchStruct * mStruct = new StitchStruct();

			mStruct->m_suitStitchId = mSuitStitchIds[j];

			m_StitchsuitIds.Add(mStruct);
		}
	}
}
