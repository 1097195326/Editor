// Fill out your copyright notice in the Description page of Project Settings.

#include "FabricColorComponent.h"
#include "StyleFabricColorC.h"
#include "StyleFabricColorR.h"
#include "SuitFabricColorT.h"
#include "GPathTool.h"
#include "GFileHelper.h"

FabricColorComponent::FabricColorComponent()
{
}

FabricColorComponent::~FabricColorComponent()
{
}

void FabricColorComponent::OnLoad()
{
	m_componentTag = EnumComponentType::FABRICCOLOR_COMPONENT;
}

void FabricColorComponent::OnRelease()
{
	m_textureArray.Empty();
}

void FabricColorComponent::OnReleaseRender()
{
	m_textureArray.Empty();
}

void FabricColorComponent::OnStartRender()
{
	SuitFabricColorT * mColorT = m_dataC->GetBaseT<SuitFabricColorT>();

	if (nullptr != mColorT)
	{
		m_color = mColorT->m_RGB;

		m_hexRgb = mColorT->m_hexRgb;
	}
	LoadTexture();
}

void FabricColorComponent::OnStartSkeletalRender()
{
	SuitFabricColorT * mColorT = m_dataC->GetBaseT<SuitFabricColorT>();

	if (nullptr != mColorT)
	{
		m_color = mColorT->m_RGB;

		m_hexRgb = mColorT->m_hexRgb;
	}

	LoadTexture();
}

void FabricColorComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	m_dataC = new StyleFabricColorC();

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);
}

TArray<FString> FabricColorComponent::GetColorTexture()
{
	SuitFabricColorT * mColorT = m_dataC->GetBaseT<SuitFabricColorT>();
	if (nullptr != mColorT)
		return mColorT->m_textureUrlArray;

	TArray<FString> mPathArray;
	return mPathArray;
}

FString FabricColorComponent::GetMaterialUri()
{
	SuitFabricColorT * mColorT = m_dataC->GetBaseT<SuitFabricColorT>();
	if (nullptr != mColorT)
		return mColorT->m_materialUrl;
	return TEXT("");
}

void FabricColorComponent::LoadTexture()
{
	m_textureArray.Empty();

	TArray<FString> mPathArray = GetColorTexture();

	for (int32 i = 0; i < mPathArray.Num(); i++)
	{
		FString mTexturePath = mPathArray[i];

		FString mSavePath = GPathTool::GetHttpPath(mTexturePath);

		UTexture2D * mTexture2d = m_owner->GetTextureManager()->GetTexture2dByPath(mSavePath);

		if (nullptr != mTexture2d)
		{
			m_textureArray.Add(mTexture2d);
		}
	}
}
