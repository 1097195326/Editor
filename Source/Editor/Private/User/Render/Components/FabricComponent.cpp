// Fill out your copyright notice in the Description page of Project Settings.

#include "FabricComponent.h"
#include "StyleFabricC.h"
#include "FabricColorComponent.h"
#include "StyleFabricR.h"
#include "TSuitFabricColorLibraryManager.h"
#include "SuitFabricColorLibraryT.h"
#include "StyleFabricColorR.h"
#include "LoadAssetTool.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "SuitFabricT.h"
#include "GPathTool.h"
#include "SliceComponent.h"
#include "FabricColorComponent.h"
#include "RStyleFabricColorManager.h"

FabricComponent::FabricComponent()
{
}

FabricComponent::~FabricComponent()
{
}

void FabricComponent::InitMaterial(UMaterialInstanceDynamic * mMaterial)
{
	if (GEnumUseColorResource::USE_COLOR_RGB == CheckUseColorMat() || GEnumUseColorResource::USE_COLOR_MATERIAL == CheckUseColorMat())
	{
		if (nullptr == m_material)
			return;
		SuitFabricT * mDataT = m_dataC->GetBaseT<SuitFabricT>();

		// 获取颜色.
		TArray<IComponent*> mFabricColorArray;

		GetComponentByTag(EnumComponentType::FABRICCOLOR_COMPONENT, mFabricColorArray);

		// 材质法线贴图.
		if (m_normalTexture)
		{
			mMaterial->SetTextureParameterValue(FName("Normal"), m_normalTexture);
		}
		else
		{
			mMaterial->SetTextureParameterValue(FName("Normal"), nullptr);
		}

		// 材质上的maskurl.
		if (m_maskTexture) {
			mMaterial->SetTextureParameterValue(FName("Mask"), m_maskTexture);
		}
		else {
			mMaterial->SetTextureParameterValue(FName("Mask"), nullptr);
		}

		// 材质上的metal.
		if (m_metal) {
			mMaterial->SetTextureParameterValue(FName("MetallicTex"), m_metal);
		}
		else {
			mMaterial->SetTextureParameterValue(FName("MetallicTex"), nullptr);
		}

		// 材质上的Roughness.
		if (m_roughness) {
			mMaterial->SetTextureParameterValue(FName("RoughnessTex"), m_roughness);
		}
		else {
			mMaterial->SetTextureParameterValue(FName("RoughnessTex"), nullptr);
		}

		mMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor::White);
	}
	
}

void FabricComponent::UpdateUvScale(FVector2D _uvScale, UMaterialInstanceDynamic * _material)
{
	if (GEnumUseColorResource::USE_COLOR_RGB != CheckUseColorMat())
	{
		return;
	}
	if (!_uvScale.IsZero()) 
	{
		FLinearColor arg;

		arg.R = _uvScale.X;

		arg.G = _uvScale.Y;

		if (arg.R > 0.001f && arg.G > 0.001f) 
		{
			_material->SetVectorParameterValue(FName("FabricTexScale"), arg);
		}
		else 
		{
			_material->SetVectorParameterValue(FName("FabricTexScale"), FLinearColor::Transparent);
		}
	}
}


void FabricComponent::UpdateStitchFabric(UTexture * _baseTexture, UTexture * _normalTexture, UMaterialInstanceDynamic * _material)
{
	if (_baseTexture) {
		_material->SetTextureParameterValue(FName("PatternColor"), _baseTexture);
	}
	else {
		_material->SetTextureParameterValue(FName("PatternColor"), nullptr);
	}
	if (_normalTexture) {
		_material->SetTextureParameterValue(FName("NormalMap"), _normalTexture);
	}
	else {
		_material->SetTextureParameterValue(FName("NormalMap"), nullptr);
	}

	FabricColorComponent * mColorComponent = nullptr;

	TArray<IComponent*> mChildComponentArray;
	
	GetComponentByTag(EnumComponentType::FABRICCOLOR_COMPONENT, mChildComponentArray);

	if (mChildComponentArray.Num() > 0)
	{
		mColorComponent = (FabricColorComponent *)mChildComponentArray[0];
	}
		

	_material->SetVectorParameterValue(FName("Color"), mColorComponent->m_color);
}

void FabricComponent::SetDefaultColorTexture(UMaterialInstanceDynamic * _material)
{
	for (int32 i = 0; i < m_colorTextureCount; i++)
	{
		FString mVar = FString::Printf(TEXT("ColorTexture%d"), i);

		FName mVarFname = FName(*mVar);

		_material->SetTextureParameterValue(mVarFname, GetBlankTexture());
	}
}

void FabricComponent::UpdateMateriasTexture(PatternComponent * _patternComponent, UMaterialInstanceDynamic * _material,float _originalSize)
{
	FabricColorComponent * mColorComponent = (FabricColorComponent *)GetFabricColorComponent();

	SuitFabricT * mdataT = m_dataC->GetBaseT<SuitFabricT>();

	// 材质上的base pattern url.
	if (m_basePattern) {
		_material->SetTextureParameterValue(FName("PatternColor"), m_basePattern);

		_material->SetVectorParameterValue(FName("PatternTexScale"), FLinearColor::White);
	}
	else 
	{	
		if (_patternComponent != nullptr && mdataT->m_CanPattern==true)
		{
			const UTexture2D *pColorTex = _patternComponent->m_texture2d;

			UTexture* pColorTexture = (UTexture*)pColorTex;

			if (pColorTexture != nullptr && pColorTexture->IsValidLowLevel()) 
			{
				_material->SetTextureParameterValue(FName("PatternColor"), pColorTexture);

				_material->SetVectorParameterValue(FName("BaseColor"), FLinearColor::White);

				FVector2D UVScaleBase = FVector2D(_patternComponent->GetWidthT(), _patternComponent->GetHeightT());

				FVector2D UVScaleUser = _patternComponent->GetUVScaleUser();
				FLinearColor arg;
				float temp_OriginalSize = _originalSize;
				arg.R = temp_OriginalSize / UVScaleBase.X*UVScaleUser.X;
				arg.G = temp_OriginalSize / UVScaleBase.Y*UVScaleUser.Y;

				if (arg.R > 0.001f && arg.G > 0.001f) 
				{
					_material->SetVectorParameterValue(FName("PatternTexScale"), arg);
				}
				else 
				{
					_material->SetVectorParameterValue(FName("PatternTexScale"), FLinearColor::Transparent);
				}
			}
		}
		else 
		{
			_material->SetVectorParameterValue(FName("PatternTexScale"), FLinearColor::Transparent);

			_material->SetTextureParameterValue(FName("PatternColor"), GetBlankTexture());
		}
	}

	if (nullptr != _patternComponent)
		UpdateUvScale(_patternComponent->GetUVScaleUser(), _material);

	// 判断颜色跟花型都没有的时候该怎么处理.
	if (m_basePattern == NULL && _patternComponent->m_texture2d == nullptr && nullptr != mColorComponent && m_colorTextureCount==0)
	{
		_material->SetVectorParameterValue(FName("BaseColor"), FColor::FromHex(mColorComponent->m_hexRgb));
		
		_material->SetVectorParameterValue(FName("PatternTexScale"), FLinearColor::Transparent);

		_material->SetTextureParameterValue(FName("PatternColor"), GetBlankTexture());
	}

	// 重置花色渲染的贴图.
	SetDefaultColorTexture(_material);

	m_colorTextureCount = mColorComponent->m_textureArray.Num();

	if (m_colorTextureCount>0)
	{
		_material->SetVectorParameterValue(FName("BaseColor"), FLinearColor::White);

		_material->SetTextureParameterValue(FName("PatternColor"), GetBlankTexture());
	}

	for (int32 i = 0; i <m_colorTextureCount; i++)
	{
		UTexture2D * mTexture2d = mColorComponent->m_textureArray[i];

		FString mVar = FString::Printf(TEXT("ColorTexture%d"), i);

		FString mVarUVScale = FString::Printf(TEXT("ColorTextureScale%d"), i);

		FName mVarFname = FName(*mVar);

		if (nullptr != mTexture2d)
		{
			FLinearColor arg;
			arg.R = _originalSize / (1024.0f/72.0f*2.54f);
			arg.G = _originalSize / (1024.0f / 72.0f*2.54f);
			_material->SetVectorParameterValue(FName("PatternTexScale"), arg);

			_material->SetTextureParameterValue(mVarFname, mTexture2d);
		}
		else
		{
			_material->SetTextureParameterValue(mVarFname, GetBlankTexture());
		}
	}

	// 设置法线贴图uvScale.
	if (mdataT->m_normalOriginalSize != 0)
	{
		FLinearColor arg;

		float temp_OriginalSize = _originalSize/ mdataT->m_normalOriginalSize;

		arg.R = temp_OriginalSize;

		arg.G = temp_OriginalSize;

		_material->SetVectorParameterValue(FName("NormalUV"), arg);

		_material->SetVectorParameterValue(FName("OpacityUV"), arg);
	}
}

UTexture2D * FabricComponent::GetBlankTexture()
{
	FString mPath = GPathTool::GetBaseResourcePath(TEXT("Blank_TEX.Blank_TEX"));
	UTexture2D * mTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mPath);
	return mTexture;
}

void FabricComponent::OnLoad()
{
	m_componentTag = EnumComponentType::FABRIC_COMPONENT;
}

void FabricComponent::OnRelease()
{
	m_materiaInstanceArray.Empty();

	if (nullptr != m_material)
	{
		m_material = nullptr;
	}

	if (nullptr != m_normalTexture)
	{
		m_normalTexture = nullptr;
	}

	if (nullptr != m_maskTexture)
	{
		m_maskTexture = nullptr;
	}

	if (nullptr != m_metal)
	{
		m_metal = nullptr;
	}

	if (nullptr != m_roughness)
	{
		m_roughness = nullptr;
	}

	if (nullptr != m_basePattern)
	{
		m_basePattern = nullptr;
	}

	m_materiaInstanceArray.Empty();
}

void FabricComponent::OnReleaseRender()
{
	m_materiaInstanceArray.Empty();

	if (nullptr != m_material)
	{
		m_material = nullptr;
	}
}

void FabricComponent::OnStartRender()
{
	SuitFabricT * mdataT = m_dataC->GetBaseT<SuitFabricT>();

	FString mResourcePath = GetFabricPath();

	if (nullptr != m_material && !m_currentMaterialPath.Equals(mResourcePath))
	{
		m_material = nullptr;

		m_materiaInstanceArray.Empty();
	}

	m_currentMaterialPath = mResourcePath;

	if (nullptr == m_material)
	{
		m_material = ULoadAssetTool::GetInstance()->LoadUAsset<UMaterialInterface>(m_currentMaterialPath);

		if (nullptr == m_material)
			return;

		m_dataC->m_renderAsset = mdataT->m_MaterialTemplateUrl;
	}

	if (GEnumUseColorResource::USE_COLOR_RGB == CheckUseColorMat())
	{
		FString mNormalPath = GPathTool::GetFabricPath(mdataT->m_NormalUrl);

		FString mMaskPath = GPathTool::GetFabricPath(mdataT->m_MaskUrl);

		FString mMetalPath = GPathTool::GetFabricPath(mdataT->m_MetalUrl);

		FString mRoughnessPath = GPathTool::GetFabricPath(mdataT->m_RoughnessUrl);

		FString mBasePattern = GPathTool::GetFabricPath(mdataT->m_BasePatternUrl);

		m_normalTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mNormalPath);

		m_maskTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mMaskPath);

		m_metal = m_owner->GetTextureManager()->GetTexture2dInContent(mMetalPath);

		m_roughness = m_owner->GetTextureManager()->GetTexture2dInContent(mRoughnessPath);

		m_basePattern = m_owner->GetTextureManager()->GetTexture2dInContent(mBasePattern);
	}
}

void FabricComponent::OnStartSkeletalRender()
{
	SuitFabricT * mdataT = m_dataC->GetBaseT<SuitFabricT>();

	FString mResourcePath = GetFabricPath();

	if (nullptr != m_material)
	{
		m_material = nullptr;
	}

	if (nullptr == m_material)
	{
		m_material = ULoadAssetTool::GetInstance()->LoadUAsset<UMaterialInterface>(mResourcePath);

		if (nullptr == m_material)
			return;

		m_dataC->m_renderAsset = mdataT->m_MaterialTemplateUrl;
	}
	if (GEnumUseColorResource::USE_COLOR_RGB == CheckUseColorMat())
	{
		FString mNormalPath = GPathTool::GetFabricPath(mdataT->m_NormalUrl);

		FString mMaskPath = GPathTool::GetFabricPath(mdataT->m_MaskUrl);

		FString mMetalPath = GPathTool::GetFabricPath(mdataT->m_MetalUrl);

		FString mRoughnessPath = GPathTool::GetFabricPath(mdataT->m_RoughnessUrl);

		FString mBasePattern = GPathTool::GetFabricPath(mdataT->m_BasePatternUrl);

		m_normalTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mNormalPath);

		m_maskTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mMaskPath);

		m_metal = m_owner->GetTextureManager()->GetTexture2dInContent(mMetalPath);

		m_roughness = m_owner->GetTextureManager()->GetTexture2dInContent(mRoughnessPath);

		m_basePattern = m_owner->GetTextureManager()->GetTexture2dInContent(mBasePattern);
	}
}

void FabricComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	StyleFabricC * mDataC = new StyleFabricC();

	m_dataC = mDataC;

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);

	// 创建花色Component.
	FabricColorComponent * mComponent = new FabricColorComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(mDataC->m_styleColorId, mDataC->m_suitColorId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);
}

FString FabricComponent::GetFabricPath()
{
	// 判断是否从花色拿材质.
	GEnumUseColorResource mUseColorType = CheckUseColorMat();

	SuitFabricT * mdataT = m_dataC->GetBaseT<SuitFabricT>();
	
	FabricColorComponent * mColorComponent = (FabricColorComponent *)GetFabricColorComponent();

	FString mResourcePath;

	switch (mUseColorType)
	{
	case USE_COLOR_RGB:
	{
		if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
		{
			mResourcePath = GPathTool::GetFabricPath(mdataT->m_MaterialTemplateUrl);
		}
		else
		{
			mResourcePath = GPathTool::GetFabricPath(mdataT->m_MaterialTemplateUrl);
		}
		break;
	}
	case USE_COLOR_MATERIAL:
	{
		mResourcePath = GPathTool::GetFabricPath((mColorComponent->GetMaterialUri()));
		break;
	}
	default:
	{
		if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
		{
			mResourcePath = GPathTool::GetFabricPath(mdataT->m_MaterialTemplateUrl);
		}
		else
		{
			mResourcePath = GPathTool::GetFabricPath(mdataT->m_MaterialTemplateUrl);
		}
		break;
	}
	}
	
	return mResourcePath;
}

void FabricComponent::GetColorTexturePath(TArray<FString> & _pathArray)
{
	FabricColorComponent * mColorComponent = (FabricColorComponent *)GetFabricColorComponent();
	_pathArray = mColorComponent->GetColorTexture();
}

GEnumUseColorResource FabricComponent::CheckUseColorMat()
{
	FabricColorComponent * mColorComponent = (FabricColorComponent *)GetFabricColorComponent();

	if (!mColorComponent->GetMaterialUri().IsEmpty())
	{
		return GEnumUseColorResource::USE_COLOR_MATERIAL;
	}
	else if (mColorComponent->GetColorTexture().Num()>0)
	{
		return GEnumUseColorResource::USE_COLOR_TEXTURE;
	}
	return GEnumUseColorResource::USE_COLOR_RGB;
}

BaseComponent * FabricComponent::GetFabricColorComponent()
{
	FabricColorComponent * mColorComponent = nullptr;

	TArray<IComponent*> mChildComponentArray;

	GetComponentByTag(EnumComponentType::FABRICCOLOR_COMPONENT, mChildComponentArray);

	if (mChildComponentArray.Num() > 0)
		mColorComponent = (FabricColorComponent *)mChildComponentArray[0];

	return mColorComponent;
}

