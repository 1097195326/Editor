// Fill out your copyright notice in the Description page of Project Settings.

#include "PiecesComponent.h"
#include "StyleComponentC.h"
#include "RStyleStitchManager.h"
#include "StyleComponentR.h"
#include "StyleStitchR.h"
#include "StyleAccessoriesR.h"
#include "SliceComponent.h"
#include "AccessoriesComponent.h"
#include "StyleSliceR.h"
#include "StitchComponent.h"
#include "SuitComponentT.h"

PiecesComponent::PiecesComponent()
{
}

PiecesComponent::~PiecesComponent()
{
}

void PiecesComponent::OnLoad()
{
	m_componentTag = EnumComponentType::PIECES_COMPONENT;
}

void PiecesComponent::OnRelease()
{

}

void PiecesComponent::OnStartRender()
{

}

void PiecesComponent::OnStartSkeletalRender()
{

}

void PiecesComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	m_dataC = new StyleComponentC();

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);

	// 线迹数组.
	CreateStitchComponents();

	// 部片数组.
	CreateSliceComponents();

	// 辅料数组.
	CreateAccessoriesComponents();
}

void PiecesComponent::CreateAccessoriesComponents()
{
	StyleComponentC * mDataC = (StyleComponentC *)m_dataC;

	for (int32 i=0;i<mDataC->m_AccessoryIds.Num();i++)
	{
		CAccessoryStruct * mStruct = mDataC->m_AccessoryIds[i];

		// 创建Component.
		AccessoriesComponent * mComponent = new AccessoriesComponent();

		mComponent->GLoad(m_owner);

		mComponent->InitData(mStruct->m_styleAccessoryId, mStruct->m_suitAccessoryId, m_owner->CreateComponentID(), this);

		AddChildComponent(mComponent);
	}
}
// 批量修改片上用的花型面料花色
void PiecesComponent::ChangeSliceChildComponent(UsedFabricStruct & _usedFabricStruct)
{
	// 只有父组件是部位的时候才执行.
	if (m_parentComponent->GetComponentTag() != EnumComponentType::PART_COMPONENT)
		return;

	for (int32 i=0;i<m_childComponents.Num();i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		// 只获取slice.
		if (mComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
		{
			mComponent->ChangeRenderTargetTid(_usedFabricStruct);
		}
	}
}

void PiecesComponent::CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	BaseComponent * mParentComponent = (BaseComponent *)this->GetParentComponent();
	
	if (mParentComponent->GetComponentTag() == EnumComponentType::PART_COMPONENT)
	{
		CreateStitchJson(_saveInfoObject);

		CreateAccessoriesJson(_saveInfoObject);
	}
	
	// 部片.
	TArray<TSharedPtr<FJsonValue>> mSliceArray;

	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		if (mComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
		{
			TSharedPtr<FJsonObject> t_value = MakeShareable(new FJsonObject);

			mComponent->CreateJson(t_value);

			TSharedPtr<FJsonValue> EntryValue = MakeShareable(new FJsonValueObject(t_value));

			mSliceArray.Add(EntryValue);
		}
	}
	_saveInfoObject->SetArrayField(TEXT("partComCollections"), mSliceArray);

}

void PiecesComponent::CreateStitchJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	// 线迹.
	TArray<TSharedPtr<FJsonValue>> mSliceArray;

	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		if (mComponent->GetComponentTag() == EnumComponentType::STITCH_COMPONENT)
		{
			TSharedPtr<FJsonObject> t_value = MakeShareable(new FJsonObject);

			mComponent->CreateJson(t_value);

			TSharedPtr<FJsonValue> EntryValue = MakeShareable(new FJsonValueObject(t_value));

			mSliceArray.Add(EntryValue);
		}
	}
	_saveInfoObject->SetArrayField(TEXT("buildinDecorationStitchs"), mSliceArray);
}

void PiecesComponent::CreateAccessoriesJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	// 辅料.
	TArray<TSharedPtr<FJsonValue>> mAccessoriesArray;

	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		if (mComponent->GetComponentTag() == EnumComponentType::ACCESSORIES_COMPONENT)
		{
			TSharedPtr<FJsonObject> t_value = MakeShareable(new FJsonObject);

			mComponent->CreateJson(t_value);

			TSharedPtr<FJsonValue> EntryValue = MakeShareable(new FJsonValueObject(t_value));

			mAccessoriesArray.Add(EntryValue);
		}
	}
	_saveInfoObject->SetArrayField(TEXT("suitAccessoriesPartCollections"), mAccessoriesArray);
}

void PiecesComponent::CreateSliceComponents()
{
	StyleComponentC * mDataC = (StyleComponentC *)m_dataC;

	for (int32 i = 0; i < mDataC->m_SliceIds.Num(); i++)
	{
		SliceStruct * mStruct = mDataC->m_SliceIds[i];

		// 创建Component.
		SliceComponent * mComponent = new SliceComponent();

		mComponent->GLoad(m_owner);

		mComponent->InitData(mStruct->m_styleSliceId, mStruct->m_suitSliceId, m_owner->CreateComponentID(), this);

		AddChildComponent(mComponent);
	}
}


void PiecesComponent::CreateStitchComponents()
{
	StyleComponentC * mDataC = (StyleComponentC *)m_dataC;

	for (int32 i = 0; i < mDataC->m_StitchsuitIds.Num(); i++)
	{
		StitchStruct * mStruct = mDataC->m_StitchsuitIds[i];

		// 创建Component.
		StitchComponent * mComponent = new StitchComponent();

		AddChildComponent(mComponent);

		mComponent->GLoad(m_owner);

		mComponent->InitData(mStruct->m_styleStitchId, mStruct->m_suitStitchId, m_owner->CreateComponentID(), this);
	}
}
