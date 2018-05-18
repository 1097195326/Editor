// Fill out your copyright notice in the Description page of Project Settings.

#include "SuitFabricColorT.h"

SuitFabricColorT::SuitFabricColorT()
{
}

SuitFabricColorT::~SuitFabricColorT()
{
}

void SuitFabricColorT::OnLoad()
{

}

void SuitFabricColorT::OnRelease()
{

}

void SuitFabricColorT::OnInitWithJson(const TSharedPtr<FJsonObject> & _jsonObj)
{
	m_Tid =_jsonObj->GetNumberField(TEXT("goodsId"));
	m_IsBuy=_jsonObj->GetBoolField(TEXT("isBuy"));
	m_PantoneCode = _jsonObj->GetStringField(TEXT("pantoneCode"));
	m_ProductCode = _jsonObj->GetStringField(TEXT("productCode"));
	
	m_sn = _jsonObj->GetStringField(TEXT("sn"));
	_jsonObj->TryGetStringField(TEXT("icon"), m_icon);

	_jsonObj->TryGetStringField(TEXT("material"), m_materialUrl);

	const TArray<TSharedPtr<FJsonValue>>& mTextureArray = _jsonObj->GetArrayField(TEXT("goodsImageList"));
	for (int32 i = 0; i < mTextureArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> mTempJson = mTextureArray[i];

		m_textureUrlArray.Add(mTempJson->AsString());
	}

	const TSharedPtr<FJsonObject>& mObj = _jsonObj->GetObjectField(TEXT("rGB"));
	//!!!注意类型为String强转可能会出错.
	m_RGB.A = mObj->GetNumberField(TEXT("a"));
	m_RGB.B = mObj->GetNumberField(TEXT("b"));
	m_RGB.G = mObj->GetNumberField(TEXT("g"));
	m_RGB.R = mObj->GetNumberField(TEXT("r"));
	mObj->TryGetStringField(TEXT("hexRgb"), m_hexRgb);
}
