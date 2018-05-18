// Fill out your copyright notice in the Description page of Project Settings.

#include "PartComponent.h"
#include "StylePartC.h"
#include "StyleComponentR.h"
#include "RStyleComponentManager.h"
#include "SuitComponentT.h"
#include "StyleSliceC.h"
#include "StyleComponent.h"
#include "SliceComponent.h"
#include "SuitSliceT.h"
#include "StylePartR.h"
#include "TSuitFabricLibraryManager.h"
#include "PiecesComponent.h"
#include "SuitFabricLibraryT.h"
#include "TSuitComponentManager.h"
#include "SuitComponentT.h"
#include "SuitPartT.h"

PartComponent::PartComponent()
{
}

PartComponent::~PartComponent()
{
}

void PartComponent::OnLoad()
{
	m_componentTag = EnumComponentType::PART_COMPONENT;
}

void PartComponent::OnRelease()
{

}

void PartComponent::OnStartRender()
{
	// 这里没有渲染相关的.
}

void PartComponent::OnStartSkeletalRender()
{

}

void PartComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	StylePartC * mDataC = new StylePartC();

	m_dataC = mDataC;

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);

	// 部位对应的部件只能有一个，所以component只有一个..StyleComponentC.
	PiecesComponent * mComponent = new PiecesComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(mDataC->m_styleComponentId, mDataC->m_suitComponentId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);
}
// 换部件.
void PartComponent::ChangePiecesCompont(int32 _targetTid)
{
	// 1.判断继承面料.
	InheritFabricStruct mInheritFabricStruct;

	bool mFindInheritFabricStruct = false;

	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		if (mComponent->GetComponentTag() == EnumComponentType::PIECES_COMPONENT)
		{
			// 先获取面料信息，为了给新的部件赋值.
			for (int32 j = 0; j< mComponent->GetChildComponents().Num(); j++)
			{
				BaseComponent * mSliceComponent = (BaseComponent *)mComponent->GetChildComponents()[j];

				if (mSliceComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
				{
					// 获取要继承的参数.
					if (!mFindInheritFabricStruct)
					{
						mFindInheritFabricStruct = true;

						StyleSliceC * mSliceC = (StyleSliceC *)mSliceComponent->GetDataC();

						mInheritFabricStruct.m_angle = mSliceC->m_angle;

						mInheritFabricStruct.m_suitPatternId = mSliceC->m_suitPatternId;

						mInheritFabricStruct.m_suitColorId = mSliceC->m_suitColorId;

						mInheritFabricStruct.m_suitFabricId = mSliceC->m_suitFabricId;

						mInheritFabricStruct.m_offset = mSliceC->m_offset;

						mInheritFabricStruct.m_uVScaleUser = mSliceC->m_uVScaleUser;
						break;
					}
					
				}
				
			}
			// 释放掉.
			mComponent->GRelease();
		}
	}

	StylePartC * mDataC = (StylePartC *)m_dataC;

	mDataC->m_suitComponentId = _targetTid;

	mDataC->m_styleComponentId = 0;

	PiecesComponent * mComponent = new PiecesComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(mDataC->m_styleComponentId, mDataC->m_suitComponentId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);

	// 继承面料.
	InheritFabric(mComponent, mInheritFabricStruct);
	

	// 通知stylecomponent 换AB类型,只有主部位才替换.
	SuitPartT * mPartT = m_dataC->GetBaseT<SuitPartT>();

	if (mPartT->m_IsMainPart == 1)
	{
		SuitComponentT * mComponentT = mComponent->GetDataC()->GetBaseT<SuitComponentT>();

		int32 mAdapterType = mComponentT->m_AdapterType;


		StyleComponent * mStyleComponent = (StyleComponent *)m_parentComponent->GetParentComponentByTag(EnumComponentType::STYLE_COMPONENT);

		mStyleComponent->ChangeAdapterType(mAdapterType);
	}
}

void PartComponent::ChangePiecesComponentByAdapterType(int32 _AdapterType)
{
	InheritFabricStruct mInheritFabricStruct;

	// 获取所有的T数据.
	SuitPartT * mPartT = m_dataC->GetBaseT<SuitPartT>();

	int32 mTargetComponentTid = 0;

	for (int32 i = 0; i < mPartT->m_SuitComponentIds.Num(); i++)
	{
		int32 mComponentTid = mPartT->m_SuitComponentIds[i];

		SuitComponentT * mComponentT = TSuitComponentManager::GetInstance()->GetTemplateByPID<SuitComponentT>(mComponentTid);

		if (mComponentT->m_AdapterType == _AdapterType)
		{
			mTargetComponentTid = mComponentT->GetTid();

			break;;
		}
	}

	if (mTargetComponentTid == 0)
		return;

	bool mFindInheritFabricStruct = false;

	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		if (mComponent->GetComponentTag() == EnumComponentType::PIECES_COMPONENT)
		{
			// 在判断_AdapterType 是否一样.
			SuitComponentT * mSuitComponentT = mComponent->GetDataC()->GetBaseT<SuitComponentT>();
			if (mSuitComponentT->m_AdapterType == _AdapterType)
			{
				return;
			}

			// 先获取面料信息，为了给新的部件赋值.
			for (int32 j = 0; j < mComponent->GetChildComponents().Num(); j++)
			{
				BaseComponent * mSliceComponent = (BaseComponent *)mComponent->GetChildComponents()[j];

				if (mSliceComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
				{
					// 获取要继承的参数.
					if (!mFindInheritFabricStruct)
					{
						mFindInheritFabricStruct = true;

						StyleSliceC * mSliceC = (StyleSliceC *)mSliceComponent->GetDataC();

						mInheritFabricStruct.m_angle = mSliceC->m_angle;

						mInheritFabricStruct.m_suitPatternId = mSliceC->m_suitPatternId;

						mInheritFabricStruct.m_suitColorId = mSliceC->m_suitColorId;

						mInheritFabricStruct.m_suitFabricId = mSliceC->m_suitFabricId;

						mInheritFabricStruct.m_offset = mSliceC->m_offset;

						mInheritFabricStruct.m_uVScaleUser = mSliceC->m_uVScaleUser;
					}
					// 释放掉.
					mComponent->GRelease();
				}
			}
		}
	}

	StylePartC * mDataC = (StylePartC *)m_dataC;

	mDataC->m_suitComponentId = mTargetComponentTid;

	mDataC->m_styleComponentId = 0;

	PiecesComponent * mComponent = new PiecesComponent();

	mComponent->GLoad(m_owner);    

	mComponent->InitData(mDataC->m_styleComponentId, mDataC->m_suitComponentId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);

	// 继承面料.
	InheritFabric(mComponent, mInheritFabricStruct);
}



void PartComponent::InheritFabric(IComponent * mComponent, InheritFabricStruct & mInheritFabricStruct)
{
	for (int32 i = 0; i < mComponent->GetChildComponents().Num(); i++)
	{
		SliceComponent * mSliceComponent = (SliceComponent *)mComponent->GetChildComponents()[i];

		if (mSliceComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
		{
			// 获取要继承的参数.
			StyleSliceC * mSliceC = (StyleSliceC *)mSliceComponent->GetDataC();

			SuitSliceT * mSliceT = mSliceC->GetBaseT<SuitSliceT>();

			// 判断是否辅料.
			bool ck = mSliceT->m_MeshUrl.Contains(TEXT("_FL"));

			if (ck)
				continue;

			// 判断面料是否存在当前片的面料库上.
			SuitFabricLibraryT * mFabricLib = TSuitFabricLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricLibraryT>(mSliceT->m_FabricLibId);

			TArray<int32> mFabricArray = mFabricLib->m_Fabrics;

			bool isInhert = true;

			for (int32 k = 0; k < mFabricArray.Num(); k++)
			{
				int32 mFabricId = mFabricArray[k];

				if (mFabricId == mInheritFabricStruct.m_suitFabricId)
				{
					isInhert = false;
					break;
				}
			}

			if(isInhert)
				continue;

			mSliceC->m_angle = mInheritFabricStruct.m_angle;

			mSliceC->m_suitPatternId = mInheritFabricStruct.m_suitPatternId;

			mSliceC->m_suitColorId = mInheritFabricStruct.m_suitColorId;

			mSliceC->m_suitFabricId = mInheritFabricStruct.m_suitFabricId;

			mSliceC->m_offset = mInheritFabricStruct.m_offset;

			mSliceC->m_uVScaleUser = mInheritFabricStruct.m_uVScaleUser;

			// 刷新数据.
			mSliceComponent->UpdateChildComponent();
		}
	}
}

void PartComponent::CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	StylePartC * mDataC = (StylePartC *)m_dataC;

	_saveInfoObject->SetNumberField(TEXT("suitComponentId"), mDataC->m_suitComponentId);

	_saveInfoObject->SetNumberField(TEXT("suitPartId"), mDataC->GetTid());

	// 部件..
	BaseComponent * mPieceComponent = (BaseComponent *) m_childComponents[0];

	mPieceComponent->CreateJson(_saveInfoObject);
}
