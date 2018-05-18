
// Fill out your copyright notice in the Description page of Project Settings.

#include "SliceComponent.h"
#include "StyleSliceC.h"
#include "FabricComponent.h"
#include "SuitSliceT.h"
#include "TSuitFabricLibraryManager.h"
#include "RStylePatternManager.h"
#include "StyleSliceR.h"
#include "SuitFabricT.h"
#include "StylePatternR.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "TSuitFabricColorManager.h"
#include "SuitPatternT.h"
#include "PatternComponent.h"
#include "TSuitFabricColorLibraryManager.h"
#include "PatternComponent.h"
#include "StyleFabricR.h"
#include "FabricComponent.h"
#include "SuitPatternLibraryT.h"
#include "FabricColorComponent.h"
#include "GPathTool.h"
#include "Editor.h"
#include "LoginModule.h"
#include "GModuleManager.h"
#include "RStyleFabricManager.h"
#include "GController.h"
#include "DesignActorRender.h"
#include "SuitFabricLibraryT.h"
#include "FabricComponent.h"
#include "LoadAssetTool.h"

SliceComponent::SliceComponent()
{
}

SliceComponent::~SliceComponent()
{
}

void SliceComponent::OnLoad()
{
	m_componentTag = EnumComponentType::SLICE_COMPONENT;

}

void SliceComponent::OnRelease()
{
	
}

void SliceComponent::OnReleaseRender()
{
	
}

USkeletalMesh * SliceComponent::GetShowSkeletalMesh(const FString & inUrl) const
{
	if (inUrl.Len() == 0)
	{
		UE_LOG(GEditorLog, Warning, TEXT("skeletal mesh url is null!"), *inUrl);
		return nullptr;
	}
	FString tmpStr = inUrl;
	int splitindex = -1;
	if (tmpStr.FindLastChar(TEXT('/'), splitindex))
	{
		tmpStr.InsertAt(splitindex + 1, m_showFolder + TEXT("/"));
	}

	USkeletalMesh *targetMesh = ULoadAssetTool::GetInstance()->LoadUAsset<USkeletalMesh>(tmpStr);
	if (targetMesh == nullptr)
	{
		UE_LOG(GEditorLog, Warning, TEXT("Can't find skeletal mesh %s"), *inUrl);
	}
	return targetMesh;
}

void SliceComponent::GetSameFabricTid(UsedFabricStruct & _usedFabricStruct)
{
	SuitSliceT * mSliceT = m_dataC->GetBaseT<SuitSliceT>();

	int32 mFabricLibTid = mSliceT->m_FabricLibId;

	SuitFabricLibraryT * mFabricLibraryT = TSuitFabricLibraryManager::GetInstance()->GetTemplateByPID<SuitFabricLibraryT>(mFabricLibTid);

	if (_usedFabricStruct.m_fabricTidArray.Num() > 0)
	{
		for (int32 j = _usedFabricStruct.m_fabricTidArray.Num()-1;j>=0;j--)
		{
			int32 mOtherFabricTid = _usedFabricStruct.m_fabricTidArray[j];

			bool isExits = false;

			for (int32 i = 0; i < mFabricLibraryT->m_Fabrics.Num(); i++)
			{
				int32 mFabricTid = mFabricLibraryT->m_Fabrics[i];

				// 判断其他struct上有没有.
				if (mOtherFabricTid == mFabricTid)
				{
					isExits = true;

					break;
				}
			}

			if (!isExits)
			{
				// 没有就删掉.
				_usedFabricStruct.m_fabricTidArray.RemoveAt(j);
			}
		}
	}
	else
	{
		for (int32 i = 0; i < mFabricLibraryT->m_Fabrics.Num(); i++)
		{
			int32 mFabricTid = mFabricLibraryT->m_Fabrics[i];

			// 判断其他struct上有没有.
			_usedFabricStruct.m_fabricTidArray.Add(mFabricTid);
		}
	}
}

bool SliceComponent::CheckFabricIsStitchOrAccessories()
{
	TArray<IComponent*> mComponentArray;

	GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mComponentArray);

	if (mComponentArray.Num() > 0)
	{
		FabricComponent* mFabricComponent = (FabricComponent*)mComponentArray[0];
		
		FString mFabricPath = mFabricComponent->GetDataC()->GetBaseT<SuitFabricT>()->m_MaterialTemplateUrl;

		if (!mFabricPath.Contains(TEXT("_Metal")) && !mFabricPath.Contains(TEXT("M_Black")) && !mFabricPath.Contains(TEXT("M_White")))
		{
			return true;
		}
	}
	return false;
}


bool SliceComponent::CheckLightFabric()
{
	TArray<IComponent*> mComponentArray;

	GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mComponentArray);

	if (mComponentArray.Num() > 0)
	{
		FabricComponent* mFabricComponent = (FabricComponent*)mComponentArray[0];

		FString mFabricPath = mFabricComponent->GetDataC()->GetBaseT<SuitFabricT>()->m_MaterialTemplateUrl;

		if (!mFabricPath.Contains(TEXT("_Metal")))
		{
			return true;
		}
	}
	return false;
}

void SliceComponent::OnStartSkeletalRender()
{
	SuitSliceT * mdataT = m_dataC->GetBaseT<SuitSliceT>();

	StyleSliceC * mDataC = (StyleSliceC*)m_dataC;

	FString mModel = GPathTool::GetSuitPath(mdataT->m_MeshUrl);

	// 判断是否创建人台模型.
	if (nullptr == m_skeletalMeshComponent)
	{
		DesignActorRender * mActorRender = (DesignActorRender *)GetOwner();

		AWalker * mPawn = mActorRender->GetPawn();

		m_skeletalMeshComponent = NewObject<USkeletalMeshComponent>(mPawn);

		m_skeletalMeshComponent->LightingChannels.bChannel2 = false;
		m_skeletalMeshComponent->LightingChannels.bChannel1 = false;
		m_skeletalMeshComponent->LightingChannels.bChannel0 = false;

		m_skeletalMeshComponent->RegisterComponent();
		m_skeletalMeshComponent->SetBoundsScale(10.0f);

		USkeletalMesh * mSkeletalMesh = GetShowSkeletalMesh(mModel);

		m_skeletalMeshComponent->SetSkeletalMesh(mSkeletalMesh);

		m_skeletalMeshComponent->SetMobility(EComponentMobility::Stationary);

		mPawn->AddSkeletalMeshComponent(m_skeletalMeshComponent);
	}

	FString mBaseTextureUrl = GPathTool::GetFabricPath(FString::Printf(TEXT("Textures/%s"), *mdataT->m_baseTextureUrl));

	if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
	{
		mBaseTextureUrl = GPathTool::GetFabricPath(FString::Printf(TEXT("Stitch/%s"), *mdataT->m_baseTextureUrl));
	}
	else
	{
		mBaseTextureUrl = GPathTool::GetFabricPath(FString::Printf(TEXT("Textures/%s"), *mdataT->m_baseTextureUrl));
	}

	m_baseTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mBaseTextureUrl);

	FString mMeshNormalPath = GPathTool::GetSuitPath(mdataT->m_meshNormalUrl);

	if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
	{
		mMeshNormalPath = GPathTool::GetFabricPath(FString::Printf(TEXT("Stitch/%s"), *mdataT->m_meshNormalUrl));
	}
	else
	{
		mMeshNormalPath = GPathTool::GetSuitPath(mdataT->m_meshNormalUrl);
	}
	m_normalTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mMeshNormalPath);

	
	m_skeletalMeshComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	m_skeletalMeshComponent->SetVisibility(true);

	// 获取所有的面料，判断要是有颜色就用颜色.没有颜色就用花型.
	TArray<IComponent*> mComponentArray;
	
	GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mComponentArray);

	if (mComponentArray.Num() > 0)
	{
		FabricComponent* mFabricComponent = (FabricComponent*)mComponentArray[0];

		FString mFabricPath = mFabricComponent->GetDataC()->GetBaseT<SuitFabricT>()->m_MaterialTemplateUrl;

		// 获取花型,要是有花型就渲染花型。没有花型就渲染颜色,在fabricicomponent里面控制这个逻辑..
		TArray<IComponent*> mPatternComponentArray;
		
		GetComponentByTag(EnumComponentType::PATTERN_COMPONENT, mPatternComponentArray);

		PatternComponent * mPatternComponent = nullptr;

		if (mPatternComponentArray.Num() > 0)
			mPatternComponent = (PatternComponent *)mPatternComponentArray[0];

		// 判断当前片是辅料的还是线迹的还是成衣的.
		int32 mIsStitchType = 0;

		if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
		{
			mIsStitchType = 1;
		}

		// 判断是否已经有面料实例.
		if (mFabricComponent->m_materiaInstanceArray.Num() == 0)
		{
			TArray< UMaterialInterface*> mArrs = m_skeletalMeshComponent->GetMaterials();

			for (int32 i = 0; i < mArrs.Num(); i++)
			{
				UMaterialInstanceDynamic* stam = (UMaterialInstanceDynamic*)(m_skeletalMeshComponent->CreateDynamicMaterialInstance(i, Cast<UMaterialInterface>(mFabricComponent->m_material)));
				
				mFabricComponent->m_materiaInstanceArray.Add(stam);
			}
		}
		if (mFabricComponent->m_materiaInstanceArray.Num() > 0)
		{
			for (int32 i = 0; i < mFabricComponent->m_materiaInstanceArray.Num(); i++)
			{
				UMaterialInstanceDynamic* stam = (UMaterialInstanceDynamic*)(mFabricComponent->m_materiaInstanceArray[i]);
				mFabricComponent->InitMaterial(stam);

				if (!CheckLightFabric()) {
					m_skeletalMeshComponent->LightingChannels.bChannel1 = true;
				}
				else
				{
					m_skeletalMeshComponent->LightingChannels.bChannel1 = false;
				}
				if (GController::m_platform == 0 || GController::m_platform == 1)
				{
					m_skeletalMeshComponent->LightingChannels.bChannel1 = true;
					m_skeletalMeshComponent->LightingChannels.bChannel0 = true;
					m_skeletalMeshComponent->LightingChannels.bChannel2 = true;
				}
		
				if (mIsStitchType == 1)
				{
					mFabricComponent->UpdateStitchFabric(m_baseTexture, m_normalTexture, stam);
				}
				else
				{
					mFabricComponent->UpdateMateriasTexture(mPatternComponent, stam, mdataT->m_OriginalSize);
				}

				m_skeletalMeshComponent->SetVectorParameterValueOnMaterials(FName("Light"), FVector(0.f, 0.f, 0.f));

				m_skeletalMeshComponent->SetScalarParameterValueOnMaterials(FName("Transparency"), 1.f);
			}
		}
	}
}

void SliceComponent::OnStartRender()
{
	SuitSliceT * mdataT = m_dataC->GetBaseT<SuitSliceT>();

	StyleSliceC * mDataC = (StyleSliceC*)m_dataC;

	FString mModel = GPathTool::GetSuitPath(mdataT->m_MeshUrl);

	// 判断是否创建人台模型.
	if (nullptr == m_staticMeshComponent)
	{
		m_staticMeshComponent = NewObject<UStaticMeshComponent>(GetOwner()->GetActor());

		m_staticMeshComponent->AttachToComponent(GetOwner()->GetActor()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

		m_staticMeshComponent->RegisterComponent();

		m_dataC->m_renderAsset = mdataT->m_MeshUrl;

		UStaticMesh * mStaticMesh = ULoadAssetTool::GetInstance()->LoadUAsset<UStaticMesh>(mModel);

  		m_staticMeshComponent->LightingChannels.bChannel2 = false;
		m_staticMeshComponent->LightingChannels.bChannel1 = false;
		m_staticMeshComponent->LightingChannels.bChannel0 = false;

		m_staticMeshComponent->SetStaticMesh(mStaticMesh);
	}
	
	FString mBaseTextureUrl = GPathTool::GetFabricPath(FString::Printf(TEXT("Textures/%s"), *mdataT->m_baseTextureUrl));

	if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
	{
		mBaseTextureUrl = GPathTool::GetFabricPath(FString::Printf(TEXT("Stitch/%s"), *mdataT->m_baseTextureUrl));
	}
	else
	{
		mBaseTextureUrl = GPathTool::GetFabricPath(FString::Printf(TEXT("Textures/%s"), *mdataT->m_baseTextureUrl));
	}

	m_baseTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mBaseTextureUrl);

	FString mMeshNormalPath = GPathTool::GetSuitPath(mdataT->m_meshNormalUrl);

	if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
	{
		mMeshNormalPath = GPathTool::GetFabricPath(FString::Printf(TEXT("Stitch/%s"), *mdataT->m_meshNormalUrl));
	}
	else
	{
		mMeshNormalPath = GPathTool::GetSuitPath(mdataT->m_meshNormalUrl);
	}
	m_normalTexture = m_owner->GetTextureManager()->GetTexture2dInContent(mMeshNormalPath);

	m_staticMeshComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));

	m_staticMeshComponent->SetVisibility(true);

	// 获取所有的面料，判断要是有颜色就用颜色.没有颜色就用花型.
	TArray<IComponent*> mComponentArray;
	
	GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mComponentArray);

	if (mComponentArray.Num() > 0)
	{
		FabricComponent* mFabricComponent = (FabricComponent*)mComponentArray[0];

		FString mFabricPath = mFabricComponent->GetDataC()->GetBaseT<SuitFabricT>()->m_MaterialTemplateUrl;

		// 获取花型,要是有花型就渲染花型。没有花型就渲染颜色,在fabricicomponent里面控制这个逻辑..
		TArray<IComponent*> mPatternComponentArray;
		
		GetComponentByTag(EnumComponentType::PATTERN_COMPONENT, mPatternComponentArray);

		PatternComponent * mPatternComponent = nullptr;

		if (mPatternComponentArray.Num() > 0)
			mPatternComponent = (PatternComponent *)mPatternComponentArray[0];

		// 判断当前片是辅料的还是线迹的还是成衣的.
		int32 mIsStitchType = 0;

		if (nullptr != GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
		{
			mIsStitchType = 1;
		}

		// 判断是否已经有面料实例.
		if (mFabricComponent->m_materiaInstanceArray.Num() == 0)
		{
			TArray< UMaterialInterface*> mArrs = m_staticMeshComponent->GetMaterials();

			for (int32 i = 0; i < mArrs.Num(); i++)
			{
				UMaterialInstanceDynamic* stam = (UMaterialInstanceDynamic*)(m_staticMeshComponent->CreateDynamicMaterialInstance(i, Cast<UMaterialInterface>(mFabricComponent->m_material)));

				mFabricComponent->m_materiaInstanceArray.Add(stam);
			}
		}
		if (mFabricComponent->m_materiaInstanceArray.Num() > 0)
		{
			for (int32 i = 0; i < mFabricComponent->m_materiaInstanceArray.Num(); i++)
			{
				UMaterialInstanceDynamic* stam = (UMaterialInstanceDynamic*)(mFabricComponent->m_materiaInstanceArray[i]);
				// 接受光的通道设置.
				if (!CheckLightFabric()) {
					m_staticMeshComponent->LightingChannels.bChannel1 = true;
				}
				else
				{
					m_staticMeshComponent->LightingChannels.bChannel1 = false;
				}
				if (GController::m_platform == 0 || GController::m_platform == 1)
				{
					m_staticMeshComponent->LightingChannels.bChannel1 = false;
					m_staticMeshComponent->LightingChannels.bChannel0 = true;
					m_staticMeshComponent->SetMobility(EComponentMobility::Static);
				}
				// 初始化所有的材质实例,只初始化一次.
				mFabricComponent->InitMaterial(stam);

				m_staticMeshComponent->SetVectorParameterValueOnMaterials(FName("Light"), FVector(0.f, 0.f, 0.f));

				m_staticMeshComponent->SetScalarParameterValueOnMaterials(FName("Transparency"), 1.f);

				// 模型法线.
				if (nullptr != m_normalTexture)
				{
					stam->SetTextureParameterValue(FName("MeshNormal"), m_normalTexture);
				}
				else
				{
					stam->SetTextureParameterValue(FName("MeshNormal"), nullptr);
				}
	
				if (mIsStitchType == 1)
				{
					mFabricComponent->UpdateStitchFabric(m_baseTexture, m_normalTexture, stam);
				}
				else
				{
					mFabricComponent->UpdateMateriasTexture(mPatternComponent, stam, mdataT->m_OriginalSize);
				}
			}
		}
	}
}

void SliceComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	StyleSliceC * mDataC = new StyleSliceC();

	m_dataC = mDataC;

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);

	// 创建花型.
	CreatePatternComponent();

	// 创建面料.
	CreateFabricComponent();

	// 判断是否用颜色还是花型.
	if (!mDataC->m_suitPatternId.IsEmpty() && !mDataC->m_suitPatternId.Equals(TEXT("SYSTEM_0")))
	{
		ChangeColorComponent(0);
	}
	else
	{
		FString mPatternId = TEXT("");

		ChangePatternComponent(mPatternId);

		ChangeColorComponent(mDataC->m_suitColorId);
	}
}

void SliceComponent::CreateFabricComponent()
{
	StyleSliceC * mDataC = (StyleSliceC *)m_dataC;

	FabricComponent * mComponent = new FabricComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(0, mDataC->m_suitFabricId, m_owner->CreateComponentID(), this);

	AddChildComponent(mComponent);
}

void SliceComponent::CreatePatternComponent()
{
	StyleSliceC * mDataC = (StyleSliceC *)m_dataC;

	PatternComponent * mComponent = new PatternComponent();

	mComponent->GLoad(m_owner);

	mComponent->InitData(0, 0, m_owner->CreateComponentID(), this);

	mComponent->ChangePatternTid(mDataC->m_suitPatternId);

	AddChildComponent(mComponent);
}

// 切换花型(片需要重写).
void SliceComponent::ChangePatternComponent(FString _targetTid)
{

	((StyleSliceC*)m_dataC)->m_suitPatternId = _targetTid;

	// 更新数据.
	TArray<IComponent *> mPatternComponentArray;
	
	GetComponentByTag(EnumComponentType::PATTERN_COMPONENT, mPatternComponentArray);

	if (mPatternComponentArray.Num() > 0)
	{
		PatternComponent * mPatternComponent = (PatternComponent *)mPatternComponentArray[0];

		mPatternComponent->ChangePatternTid(_targetTid);

		if(!_targetTid.IsEmpty())
			ChangeColorComponent(0);
	}
}
// 切换花色(片需要重写).
void SliceComponent::ChangeColorComponent(int32 _targetTid)
{
	((StyleSliceC*)m_dataC)->m_suitColorId = _targetTid;

	// 更新数据.
	TArray<IComponent *> mColorComponentArray;
	
	GetComponentByTag(EnumComponentType::FABRICCOLOR_COMPONENT, mColorComponentArray);

	if (mColorComponentArray.Num() > 0)
	{
		FabricColorComponent * mColorComponent = (FabricColorComponent *)mColorComponentArray[0];

		mColorComponent->GetDataC()->ChangeTid(_targetTid);

		if(_targetTid !=0)
			ChangePatternComponent(TEXT(""));
	}
}
// 切换面料(片需要重写).
void SliceComponent::ChangeFabricComponent(int32 _targetTid)
{
	StyleSliceC * mStyleSliceC = ((StyleSliceC*)m_dataC);

	mStyleSliceC->m_suitFabricId = _targetTid;

	// 更新数据.
	TArray<IComponent *> mFabricComponentArray;
	
	GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mFabricComponentArray);

	for (int32 i= mFabricComponentArray.Num()-1;i>=0;i--)
	{
		FabricComponent * mFabricComponent = (FabricComponent *)mFabricComponentArray[i];

		mFabricComponent->GRelease();
	}

	// 重新创建新的面料.
	CreateFabricComponent();
}
// 获取所有的子组件里用到的花型(部片类需要重写).
void SliceComponent::GetAllPatternInChildComponent(TArray<UsedFabricStruct> & _array)
{
	StyleSliceC* mDataC = (StyleSliceC*)m_dataC;

	if (mDataC->m_suitPatternId.IsEmpty())
		return;

	// 过滤是否有重复的面料.
	for (int32 i=0;i<_array.Num();i++)
	{
		UsedFabricStruct & mOtherStruct = _array[i];

		if (mOtherStruct.m_fabricTid == mDataC->m_suitFabricId && mOtherStruct.m_patternTid.Equals(mDataC->m_suitPatternId))
		{
			GetSameFabricTid(mOtherStruct);
			return;
		}
	}

	UsedFabricStruct mStruct;

	mStruct.m_fabricTid = mDataC->m_suitFabricId;

	mStruct.m_patternTid = mDataC->m_suitPatternId;

	GetSameFabricTid(mStruct);

	_array.Add(mStruct);
}


void SliceComponent::GetAllFabricInfoIn(TArray<UsedFabricStruct> & _array)
{
	if (!CheckFabricIsStitchOrAccessories())
		return;
	
	StyleSliceC* mDataC = (StyleSliceC*)m_dataC;

	SuitSliceT * mSliceT = mDataC->GetBaseT<SuitSliceT>();

	TArray<IComponent*> mComponentArray;

	GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mComponentArray);

	if (mComponentArray.Num() > 0)
	{
		FabricComponent* mFabricComponent = (FabricComponent*)mComponentArray[0];

		bool mCanPattern = mFabricComponent->GetDataC()->GetBaseT<SuitFabricT>()->m_CanPattern;

		if (mDataC->m_suitColorId == 0 && !mCanPattern)
		{
			return;
		}
	}

	// 过滤是否有重复的面料.
	for (int32 i = 0; i < _array.Num(); i++)
	{
		UsedFabricStruct & _usedFabricStruct = _array[i];

		if (mDataC->m_suitColorId == _usedFabricStruct.m_colorTid && _usedFabricStruct.m_fabricTid == mDataC->m_suitFabricId && _usedFabricStruct.m_patternTid.Equals(mDataC->m_suitPatternId))
		{
			GetSameFabricTid(_usedFabricStruct);
			return;
		}
	}

	UsedFabricStruct mStruct;

	mStruct.m_fabricTid = mDataC->m_suitFabricId;

	mStruct.m_colorTid = mDataC->m_suitColorId;

	mStruct.m_patternTid = mDataC->m_suitPatternId;

	GetSameFabricTid(mStruct);

	_array.Add(mStruct);
}

// 根据struct修改花型面料花色(部片需要重写).
void SliceComponent::ChangeRenderTargetTid(UsedFabricStruct & _usedFabricStruct)
{
	// 1.先判断面料是否一样.
	StyleSliceC* mDataC = (StyleSliceC*)m_dataC;

	if (mDataC->m_suitColorId == _usedFabricStruct.m_colorTid && _usedFabricStruct.m_fabricTid == mDataC->m_suitFabricId && _usedFabricStruct.m_patternTid.Equals(mDataC->m_suitPatternId))
	{
		// 面料一样，颜色一样.
		if (_usedFabricStruct.m_targetFabricTid != 0 && _usedFabricStruct.m_targetFabricTid != mDataC->m_suitFabricId)
		{
			ChangeFabricComponent(_usedFabricStruct.m_targetFabricTid);

			if (!_usedFabricStruct.m_targetPatternTid.IsEmpty())
			{
				ChangePatternComponent(_usedFabricStruct.m_targetPatternTid);
			}
			else if(_usedFabricStruct.m_targetColorTid != 0)
			{
				ChangeColorComponent(_usedFabricStruct.m_targetColorTid);
			}
			else
			{
				ChangeColorComponent(_usedFabricStruct.m_colorTid);
			}
		}
		else if (!_usedFabricStruct.m_targetPatternTid.IsEmpty() && !_usedFabricStruct.m_targetPatternTid.Equals(mDataC->m_suitPatternId))
		{
			ChangePatternComponent(_usedFabricStruct.m_targetPatternTid);
		}
		else if (_usedFabricStruct.m_targetColorTid!=0 && _usedFabricStruct.m_targetColorTid != mDataC->m_suitColorId)
		{
			ChangeColorComponent(_usedFabricStruct.m_targetColorTid);
		}
	}
}

// 为了继承上一次的部件信息.
void SliceComponent::UpdateChildComponent()
{
	StyleSliceC* mDataC = (StyleSliceC*)m_dataC;

	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mChildComponent = (BaseComponent *)m_childComponents[i];

		if (mChildComponent->GetComponentTag() == EnumComponentType::FABRIC_COMPONENT)
		{
			FabricComponent * mFabricComponent = (FabricComponent *)mChildComponent;

			// 继承颜色.
			for (int32 k = 0; k < mFabricComponent->GetChildComponents().Num(); k++)
			{
				FabricColorComponent * mFabricColorComponent = (FabricColorComponent *)mFabricComponent->GetChildComponents()[k];

				if (mFabricColorComponent->GetComponentTag() == EnumComponentType::FABRICCOLOR_COMPONENT)
				{
					mFabricColorComponent->GetDataC()->InitData(mFabricColorComponent->GetDataC()->GetCid(), 0, mDataC->m_suitColorId);
				}
			}

			mFabricComponent->GetDataC()->InitData(mFabricComponent->GetDataC()->GetCid(), 0, mDataC->m_suitFabricId);
		}
		else if (mChildComponent->GetComponentTag() == EnumComponentType::PATTERN_COMPONENT)
		{
			PatternComponent * mPatternComponent = (PatternComponent *)mChildComponent;

			mPatternComponent->GetDataC()->InitData(mPatternComponent->GetDataC()->GetCid(), 0, 0);

			mPatternComponent->ChangePatternTid(mDataC->m_suitPatternId);
		}
	}
}

void SliceComponent::ResetOutLine()
{    
	m_staticMeshComponent->SetVectorParameterValueOnMaterials(FName("Light"), FVector(0.f, 0.f, 0.f));

	m_staticMeshComponent->SetScalarParameterValueOnMaterials(FName("Transparency"), 1.f);

	m_staticMeshComponent->SetRenderCustomDepth(false);
}

void SliceComponent::SetOutLine()
{
	if (nullptr == GetParentComponentByTag(EnumComponentType::STITCH_COMPONENT))
	{
		m_staticMeshComponent->SetVectorParameterValueOnMaterials(FName("Light"), FVector(0.1f, 0.1f, 0.1f));

		m_staticMeshComponent->SetScalarParameterValueOnMaterials(FName("Transparency"), 1.0f);

		m_staticMeshComponent->SetRenderCustomDepth(true);

		m_staticMeshComponent->SetCustomDepthStencilValue(255);
	}
}

void SliceComponent::SetAlpa()
{
	//m_staticMeshComponent->SetVectorParameterValueOnMaterials(FName("Light"), FVector(0.f, 0.f, 0.f));

	//m_staticMeshComponent->SetScalarParameterValueOnMaterials(FName("Transparency"), 0.1f);
}

void SliceComponent::CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	StyleSliceC* mDataC = (StyleSliceC*)m_dataC;

	_saveInfoObject->SetNumberField(TEXT("colorId"), mDataC->m_suitColorId);

	_saveInfoObject->SetNumberField(TEXT("fabricId"), mDataC->m_suitFabricId);

	_saveInfoObject->SetNumberField(TEXT("suitSliceId"), mDataC->GetTid());

	_saveInfoObject->SetStringField(TEXT("patternId"), mDataC->m_suitPatternId);

	_saveInfoObject->SetNumberField(TEXT("angle"), mDataC->m_angle);

	TSharedPtr<FJsonObject> t_uvScale = MakeShareable(new FJsonObject);

	t_uvScale->SetNumberField(TEXT("x"), mDataC->m_uVScaleUser.X);

	t_uvScale->SetNumberField(TEXT("y"), mDataC->m_uVScaleUser.Y);

	_saveInfoObject->SetObjectField(TEXT("uVScaleUser"), t_uvScale);

	TSharedPtr<FJsonObject> t_offset = MakeShareable(new FJsonObject);

	t_uvScale->SetNumberField(TEXT("x"), mDataC->m_offset.X);

	t_uvScale->SetNumberField(TEXT("y"), mDataC->m_offset.Y);

	_saveInfoObject->SetObjectField(TEXT("offset"), t_uvScale);
}

FString SliceComponent::GetPatternId()
{
	StyleSliceC* mDataC = (StyleSliceC*)m_dataC;

	TArray<IComponent*> mComponentArray;

	GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mComponentArray);

	if (mComponentArray.Num() > 0)
	{
		FabricComponent* mFabricComponent = (FabricComponent*)mComponentArray[0];

		if (mFabricComponent->GetDataC()->GetBaseT<SuitFabricT>()->m_CanShowDetails)
		{
			return mDataC->m_suitPatternId;
		}
		else
		{
			return TEXT("");
		}
		
	}
	return TEXT("");
}

void SliceComponent::SetTrueLightChannel()
{
	if (GController::m_platform != ENUM_PLATFORM::GWINDOWS && GController::m_platform != ENUM_PLATFORM::GMAC)
	{
		return;
	}
	m_staticMeshComponent->LightingChannels.bChannel0 = true;
	m_staticMeshComponent->LightingChannels.bChannel2 = true;
	m_staticMeshComponent->LightingChannels.bChannel1 = true;
}

void SliceComponent::SetFalseLightChannel()
{
	if (GController::m_platform != ENUM_PLATFORM::GWINDOWS && GController::m_platform != ENUM_PLATFORM::GMAC)
	{
		return;
	}
	m_staticMeshComponent->LightingChannels.bChannel0 = false;
	m_staticMeshComponent->LightingChannels.bChannel2 = false;
	if (!CheckLightFabric()) {
		m_staticMeshComponent->LightingChannels.bChannel1 = true;
	}
	else
	{
		m_staticMeshComponent->LightingChannels.bChannel1 = false;
	}
}

void SliceComponent::CreateAccessoriesComponents()
{
	
}

