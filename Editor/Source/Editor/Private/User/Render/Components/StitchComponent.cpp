// Fill out your copyright notice in the Description page of Project Settings.

#include "StitchComponent.h"
#include "StyleStitchC.h"
#include "StylePartR.h"
#include "StyleComponentR.h"
#include "RStyleComponentManager.h"
#include "SuitStitchT.h"
#include "SuitPartT.h"
#include "PiecesComponent.h"

StitchComponent::StitchComponent()
{
}

StitchComponent::~StitchComponent()
{
}


void StitchComponent::OnLoad()
{
	m_componentTag = EnumComponentType::STITCH_COMPONENT;
}

void StitchComponent::OnRelease()
{

}

void StitchComponent::OnStartRender()
{

}

void StitchComponent::OnStartSkeletalRender()
{

}

void StitchComponent::ChangePiecesCompont(int32 _targetTid)
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

	StyleStitchC * mDataC = (StyleStitchC *)m_dataC;

	mDataC->m_SuitComponentId = _targetTid;

	mDataC->m_StyleComponentId = 0;

	PiecesComponent * mComponent = new PiecesComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(mDataC->m_StyleComponentId, mDataC->m_SuitComponentId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);
}

void StitchComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	StyleStitchC * mDataC = new StyleStitchC();

	m_dataC = mDataC;

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);

	// 部位对应的部件只能有一个，所以component只有一个..StyleComponentC.

	PiecesComponent * mComponent = new PiecesComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(mDataC->m_StyleComponentId, mDataC->m_SuitComponentId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);
}

void StitchComponent::CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	StyleStitchC * mDataC = (StyleStitchC*)m_dataC;

	_saveInfoObject->SetNumberField(TEXT("suitStitchPartId"), mDataC->GetTid());

	_saveInfoObject->SetNumberField(TEXT("suitStitchComponentId"), mDataC->m_SuitComponentId);

	// 部件..
	BaseComponent * mPieceComponent = (BaseComponent *)m_childComponents[0];

	mPieceComponent->CreateJson(_saveInfoObject);
}
