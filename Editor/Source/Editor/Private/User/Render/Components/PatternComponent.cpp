// Fill out your copyright notice in the Description page of Project Settings.

#include "PatternComponent.h"
#include "SuitPatternT.h"
#include "StylePatternC.h"
#include "StyleSliceC.h"
#include "LoadAssetTool.h"
#include "GBaseRender.h"
#include "SliceComponent.h"
#include "GPathTool.h"
#include "DownloadImageTool.h"
#include "GFileHelper.h"

PatternComponent::PatternComponent()
{
}

PatternComponent::~PatternComponent()
{
}

void PatternComponent::OnLoad()
{
	m_componentTag = EnumComponentType::PATTERN_COMPONENT;
}

void PatternComponent::OnRelease()
{
}
void PatternComponent::OnReleaseRender()
{
}

void PatternComponent::OnStartRender()
{
	ChangePatternTid(m_patternTid);

	SuitPatternT * mPatternT = m_dataC->GetBaseT<SuitPatternT>();

	if (nullptr == mPatternT)
	{
		ReleaseRender();
		return;
	}
	FString mSavePath = GPathTool::GetHttpPath(mPatternT->m_baseTextureUrl);

	m_texture2d = m_owner->GetTextureManager()->GetTexture2dByPath(mSavePath);
}

void PatternComponent::OnStartSkeletalRender()
{
	ChangePatternTid(m_patternTid);

	SuitPatternT * mPatternT = m_dataC->GetBaseT<SuitPatternT>();

	if (nullptr == mPatternT)
	{
		return;
	}
	FString mSavePath = GPathTool::GetHttpPath(mPatternT->m_baseTextureUrl);

	m_texture2d = m_owner->GetTextureManager()->GetTexture2dByPath(mSavePath);
}

FString PatternComponent::GetPatternPath()
{
	SuitPatternT * mPatternT = m_dataC->GetBaseT<SuitPatternT>();

	if(nullptr != mPatternT)
		return mPatternT->m_baseTextureUrl;

	return TEXT("");
}

void PatternComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	m_dataC = new StylePatternC();

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);
}

void PatternComponent::ChangePatternTid(const FString & _tid)
{
	m_patternTid = _tid;

	StylePatternC * mPatternC = (StylePatternC*)m_dataC;
	
	mPatternC->ChangePatternTid(_tid);

	m_texture2d = nullptr;
}

float PatternComponent::GetWidthT()
{
	StylePatternC * mPatternC = (StylePatternC*)m_dataC;

	SuitPatternT * mSuitPatternT = mPatternC->GetBaseT<SuitPatternT>();
	
	if(mSuitPatternT)
		return mSuitPatternT->m_Width;

	return 1;
}

float PatternComponent::GetHeightT()
{
	StylePatternC * mPatternC = (StylePatternC*)m_dataC;

	SuitPatternT * mSuitPatternT = mPatternC->GetBaseT<SuitPatternT>();

	if(mSuitPatternT)
		return mSuitPatternT->m_height;

	return 1;
}

FVector2D PatternComponent::GetUVScaleUser()
{
	StylePatternC * mPatternC = (StylePatternC*)m_dataC;

	return mPatternC->m_uVScaleUser;
}

FString PatternComponent::GetPatternId()
{
	SuitPatternT * mPatternT = m_dataC->GetBaseT<SuitPatternT>();

	if (nullptr != mPatternT)
		return mPatternT->m_patternId;

	return TEXT("");
}
