// Fill out your copyright notice in the Description page of Project Settings.

#include "AccessoriesComponent.h"
#include "StyleAccessoriesC.h"
#include "StylePartR.h"
#include "StyleComponentR.h"
#include "RStyleComponentManager.h"
#include "SuitPartT.h"
#include "SuitAccessoriesT.h"
#include "PiecesComponent.h"

AccessoriesComponent::AccessoriesComponent()
{
}

AccessoriesComponent::~AccessoriesComponent()
{
}

void AccessoriesComponent::OnLoad()
{
	m_componentTag = (int32)EnumComponentType::ACCESSORIES_COMPONENT;
}

void AccessoriesComponent::OnRelease()
{

}

void AccessoriesComponent::OnStartRender()
{

}

void AccessoriesComponent::OnStartSkeletalRender()
{

}

void AccessoriesComponent::ChangePiecesCompont(int32 _targetTid)
{
	// 1.判断继承面料.
	bool mFindInheritFabricStruct = false;

	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		if (mComponent->GetComponentTag() == EnumComponentType::PIECES_COMPONENT)
		{
			// 释放掉.
			mComponent->GRelease();
		}
	}

	StyleAccessoriesC * mDataC = (StyleAccessoriesC *)m_dataC;

	mDataC->m_suitComponentId = _targetTid;

	mDataC->m_styleComponentId = 0;

	PiecesComponent * mComponent = new PiecesComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(mDataC->m_styleComponentId, mDataC->m_suitComponentId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);
}

void AccessoriesComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	m_dataC = new StyleAccessoriesC();

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);

	// 部位对应的部件只能有一个，所以component只有一个..StyleComponentC.
	int32 mOutStyleComponetRid = 0;

	int32 mOutSuitComponentTid = 0;

	FindComponentId(mOutStyleComponetRid, mOutSuitComponentTid);

	PiecesComponent * mComponent = new PiecesComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(mOutStyleComponetRid, mOutSuitComponentTid, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);
}

void AccessoriesComponent::FindComponentId(int32 & _outStyleComponetRid, int32 & _outSuitComponetTid)
{
	// 获取componentR数据.
	if (nullptr != m_dataC)
	{
		StyleAccessoriesC * mAccessoriesC = (StyleAccessoriesC*)m_dataC;

		_outStyleComponetRid = mAccessoriesC->m_styleComponentId;

		_outSuitComponetTid = mAccessoriesC->m_suitComponentId;
	}
	
}

void AccessoriesComponent::CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	StyleAccessoriesC * mDataC = (StyleAccessoriesC*)m_dataC;

	_saveInfoObject->SetNumberField(TEXT("suitAccessoriesPartId"), mDataC->GetTid());

	_saveInfoObject->SetNumberField(TEXT("suitAccessoriesComponentId"), mDataC->m_suitComponentId);

	// 部件..
	BaseComponent * mPieceComponent = (BaseComponent *)m_childComponents[0];

	mPieceComponent->CreateJson(_saveInfoObject);
}
