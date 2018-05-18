// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleComponent.h"
#include "StyleC.h"
#include "StyleR.h"
#include "SuitT.h"
#include "RStylePartManager.h"
#include "LoadAssetTool.h"
#include "Catwalks.h"
#include "GPathTool.h"
#include "SuitPartT.h"
#include "SuitSliceT.h"
#include "FabricComponent.h"
#include "PartComponent.h"
#include "DesignActorRender.h"

StyleComponent::StyleComponent()
{
}

StyleComponent::~StyleComponent()
{
}

void StyleComponent::OnLoad()
{
	m_componentTag = EnumComponentType::STYLE_COMPONENT;
}

void StyleComponent::OnRelease()
{
	for (int32 i = 0; i < m_defaultModelComponentArray.Num(); i++)
	{
		UStaticMeshComponent * mStaticMeshComponent = m_defaultModelComponentArray[i];

		mStaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		mStaticMeshComponent->UnregisterComponent();

		mStaticMeshComponent->DestroyComponent();

		mStaticMeshComponent->SetStaticMesh(nullptr);
	}
	for (int32 i=0;i<m_defaultModelSkeletalComponentArray.Num();i++)
	{
		USkeletalMeshComponent * mSkeletalMeshComponent = m_defaultModelSkeletalComponentArray[i];

		mSkeletalMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		mSkeletalMeshComponent->UnregisterComponent();

		mSkeletalMeshComponent->DestroyComponent();
	}
	m_defaultModelComponentArray.Empty();
	m_defaultModelSkeletalComponentArray.Empty();
}

void StyleComponent::OnReleaseRender()
{
	for (int32 i = 0; i < m_defaultModelComponentArray.Num(); i++)
	{
		UStaticMeshComponent * mStaticMeshComponent = m_defaultModelComponentArray[i];

		mStaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		mStaticMeshComponent->UnregisterComponent();

		mStaticMeshComponent->DestroyComponent();

		mStaticMeshComponent->SetStaticMesh(nullptr);
	}
	for (int32 i = 0; i < m_defaultModelSkeletalComponentArray.Num(); i++)
	{
		USkeletalMeshComponent * mSkeletalMeshComponent = m_defaultModelSkeletalComponentArray[i];

		mSkeletalMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		mSkeletalMeshComponent->UnregisterComponent();

		mSkeletalMeshComponent->DestroyComponent();
	}
	m_defaultModelComponentArray.Empty();
	m_defaultModelSkeletalComponentArray.Empty();
}

void StyleComponent::OnStartRender()
{
	SuitT * mSuitT = m_dataC->GetBaseT<SuitT>();

	//// 1.创建模型.
	//if (m_defaultModelComponentArray.Num() == 0)
	//{
	//	for (int32 i = 0; i < mSuitT->m_defaultModelArray.Num(); i++)
	//	{
	//		FString mModel = GPathTool::GetSuitPath(mSuitT->m_defaultModelArray[i]);

	//		UStaticMeshComponent * mStaticMeshComponent = NewObject<UStaticMeshComponent>(GetOwner()->GetActor());

	//		mStaticMeshComponent->AttachToComponent(GetOwner()->GetActor()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	//		mStaticMeshComponent->RegisterComponent();

	//		UStaticMesh * mStaticMesh = ULoadAssetTool::GetInstance()->LoadUAsset<UStaticMesh>(mModel);

	//		mStaticMeshComponent->LightingChannels.bChannel2 = false;
	//		mStaticMeshComponent->LightingChannels.bChannel1 = false;
	//		mStaticMeshComponent->LightingChannels.bChannel0 = false;

	//		mStaticMeshComponent->SetStaticMesh(mStaticMesh);

	//		m_defaultModelComponentArray.Add(mStaticMeshComponent);
	//	}
	//}
	//if(m_defaultModelComponentArray.Num()>0)
	//	SetStaticMeshComponentMaterial();
}

void StyleComponent::OnStartSkeletalRender()
{
	SuitT * mSuitT = m_dataC->GetBaseT<SuitT>();

	ACatwalks * mCatWalk = (ACatwalks *)GetOwner()->GetActor();

	mCatWalk->LoadAnimation(GPathTool::GetPlatformPath(mSuitT->m_showModelAnimation));

	mCatWalk->LoadModel(GPathTool::GetPlatformPath(mSuitT->m_showModel));

	if(mSuitT->m_catwalk.Equals(TEXT("WalkingLevel")))
	{
		FVector mLocation(3944.0f, -1219.f, -10089.0f);
		mCatWalk->SetActorLocation(mLocation);

		FVector vec = FVector(0.0f, 0.0f, 90.0f);
		FQuat quat = FQuat::MakeFromEuler(vec);

		mCatWalk->SetActorRotation(quat);
	}
	else if(mSuitT->m_catwalk.Equals(TEXT("Demonstration")))
	{
		FVector mLocation(-1067.0f, -226.0f, 22.7f);
		mCatWalk->SetActorLocation(mLocation);

		FVector vec = FVector(0.0f, 0.0f, 0.0f);
		FQuat quat = FQuat::MakeFromEuler(vec);

		mCatWalk->SetActorRotation(quat);
	}

	// 1.创建模型.
	
	for (int32 i = 0; i < mSuitT->m_defaultModelArray.Num(); i++)
	{
		
		FString mModel = GPathTool::GetSuitPath(mSuitT->m_defaultModelArray[i]);

		DesignActorRender * mActorRender = (DesignActorRender *)GetOwner();

		AWalker * mPawn = mActorRender->GetPawn();

		m_skeletalMeshComponent = NewObject<USkeletalMeshComponent>(mPawn);

		m_skeletalMeshComponent->LightingChannels.bChannel2 = false;
		m_skeletalMeshComponent->LightingChannels.bChannel1 = false;
		m_skeletalMeshComponent->LightingChannels.bChannel0 = false;

		m_skeletalMeshComponent->RegisterComponent();

		m_skeletalMeshComponent->SetBoundsScale(10.0f);

		m_skeletalMeshComponent->bUseAttachParentBound = true;

		USkeletalMesh * mSkeletalMesh = GetShowSkeletalMesh(mModel);

		m_skeletalMeshComponent->SetSkeletalMesh(mSkeletalMesh);

		mPawn->AddSkeletalMeshComponent(m_skeletalMeshComponent);

		m_defaultModelSkeletalComponentArray.Add(m_skeletalMeshComponent);
	}
	if(m_defaultModelSkeletalComponentArray.Num()>0)
		SetSkeletalMeshComponentMaterial();
}

USkeletalMesh * StyleComponent::GetShowSkeletalMesh(const FString & inUrl) const
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
		FString mShowFolder = TEXT("Show");
		tmpStr.InsertAt(splitindex + 1, mShowFolder + TEXT("/"));
	}

	USkeletalMesh *targetMesh = ULoadAssetTool::GetInstance()->LoadUAsset<USkeletalMesh>(tmpStr);
	if (targetMesh == nullptr)
	{
		UE_LOG(GEditorLog, Warning, TEXT("Can't find skeletal mesh %s"), *inUrl);
	}
	return targetMesh;
}

void StyleComponent::OnInitData(int32 _rid, int32 _tid, int32 _cid)
{
	// 创建Style一个C数据.
	m_dataC = new StyleC();

	m_dataC->GLoad();

	m_dataC->InitData(_cid, _rid, _tid);

	// 遍历部位数据,并创建部位组件..
	StyleC * mStyleC = (StyleC*)m_dataC;

	for (int32 i = 0;i<mStyleC->m_PartIdArray.Num();i++)
	{
		int32 mPartRid = mStyleC->m_PartIdArray[i]->m_stylePartId;

		int32 mParTid = mStyleC->m_PartIdArray[i]->m_suitPartId;

		// 款 需要检查部位数据生成子component.
		PartComponent * mComponent = new PartComponent();

		mComponent->GLoad(m_owner);

		mComponent->InitData(mPartRid, mParTid,m_owner->CreateComponentID(),this);

		AddChildComponent(mComponent);
	}
}

void StyleComponent::CreateJson(const TSharedPtr<FJsonObject> & _saveInfoObject)
{
	StyleC * mStyleC = (StyleC*)m_dataC;

	// 款json
	TArray<TSharedPtr<FJsonValue>> _res;

	for (int32 i=0;i<m_childComponents.Num();i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		TSharedPtr<FJsonObject> t_value = MakeShareable(new FJsonObject);

		mComponent->CreateJson(t_value);

		TSharedPtr<FJsonValue> EntryValue = MakeShareable(new FJsonValueObject(t_value));

		_res.Add(EntryValue);
	}

	_saveInfoObject->SetArrayField(TEXT("suitPartCollections"), _res);
}

// 刷新子部位.
void StyleComponent::ChangeAdapterType(int32 _adapterType)
{
	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		PartComponent * mPartComponent = (PartComponent*)m_childComponents[i];

		mPartComponent->ChangePiecesComponentByAdapterType(_adapterType);
	}
}

void StyleComponent::SetStaticMeshComponentMaterial()
{
	// 获取主片.
	SliceComponent * mSliceComponent = GetMainSliceComponent();

	SuitSliceT * mSliceDataT = mSliceComponent->GetDataC()->GetBaseT<SuitSliceT>();

	TArray<IComponent*> mFabricComponentArray;

	mSliceComponent->GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mFabricComponentArray);

	FabricComponent* mFabricComponent = nullptr;

	if (mFabricComponentArray.Num() > 0)
	{
		mFabricComponent = (FabricComponent*)mFabricComponentArray[0];
	}

	TArray<IComponent*> mPatternComponentArray;

	mSliceComponent->GetComponentByTag(EnumComponentType::PATTERN_COMPONENT, mPatternComponentArray);

	PatternComponent * mPatternComponent = nullptr;

	if (mPatternComponentArray.Num() > 0)
		mPatternComponent = (PatternComponent *)mPatternComponentArray[0];

	for (int32 i = 0; i < m_defaultModelComponentArray.Num(); i++)
	{
		UStaticMeshComponent * mStaticMeshComponent = m_defaultModelComponentArray[i];

		TArray< UMaterialInterface*> mArrs = mStaticMeshComponent->GetMaterials();

		for (int32 j = 0; j < mArrs.Num(); j++)
		{
			UMaterialInstanceDynamic* stam = (UMaterialInstanceDynamic*)(mStaticMeshComponent->CreateDynamicMaterialInstance(j, Cast<UMaterialInterface>(mFabricComponent->m_material)));

			// 初始化所有的材质实例,只初始化一次.
			mFabricComponent->InitMaterial(stam);

			mFabricComponent->UpdateMateriasTexture(mPatternComponent, stam, mSliceDataT->m_OriginalSize);
		}
	}
}

void StyleComponent::SetSkeletalMeshComponentMaterial()
{
	// 获取主片.
	SliceComponent * mSliceComponent = GetMainSliceComponent();

	SuitSliceT * mSliceDataT = mSliceComponent->GetDataC()->GetBaseT<SuitSliceT>();

	TArray<IComponent*> mFabricComponentArray;

	mSliceComponent->GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, mFabricComponentArray);

	FabricComponent* mFabricComponent = nullptr;

	if (mFabricComponentArray.Num() > 0)
	{
		mFabricComponent = (FabricComponent*)mFabricComponentArray[0];
	}

	TArray<IComponent*> mPatternComponentArray;

	mSliceComponent->GetComponentByTag(EnumComponentType::PATTERN_COMPONENT, mPatternComponentArray);

	PatternComponent * mPatternComponent = nullptr;

	if (mPatternComponentArray.Num() > 0)
		mPatternComponent = (PatternComponent *)mPatternComponentArray[0];

	for (int32 i = 0; i < m_defaultModelSkeletalComponentArray.Num(); i++)
	{
		USkeletalMeshComponent * mSkeletalMeshComponent = m_defaultModelSkeletalComponentArray[i];

		TArray< UMaterialInterface*> mArrs = mSkeletalMeshComponent->GetMaterials();

		for (int32 j = 0; j < mArrs.Num(); j++)
		{
			UMaterialInstanceDynamic* stam = (UMaterialInstanceDynamic*)(mSkeletalMeshComponent->CreateDynamicMaterialInstance(j, Cast<UMaterialInterface>(mFabricComponent->m_material)));

			// 初始化所有的材质实例,只初始化一次.
			mFabricComponent->InitMaterial(stam);

			mFabricComponent->UpdateMateriasTexture(mPatternComponent, stam, mSliceDataT->m_OriginalSize);
		}
	}
}

SliceComponent * StyleComponent::GetMainSliceComponent()
{
	for (int32 i = 0; i < m_childComponents.Num(); i++)
	{
		BaseComponent * mComponent = (BaseComponent *)m_childComponents[i];

		if (mComponent->GetComponentTag() == EnumComponentType::PART_COMPONENT)
		{
			SuitPartT * mPartT = mComponent->GetDataC()->GetBaseT<SuitPartT>();

			if (mPartT->m_IsMainPart)
			{
				for (int32 j=0;j<mComponent->GetChildComponents().Num();j++)
				{
					BaseComponent * mPieceComponent = (BaseComponent *)mComponent->GetChildComponents()[j];

					if (mPieceComponent->GetComponentTag() == EnumComponentType::PIECES_COMPONENT)
					{
						for (int32 k=0;k<mPieceComponent->GetChildComponents().Num();k++)
						{
							BaseComponent * mSliceComponent = (BaseComponent *)mPieceComponent->GetChildComponents()[k];

							if (mSliceComponent->GetComponentTag() == EnumComponentType::SLICE_COMPONENT)
							{
								return (SliceComponent*)mSliceComponent;
							}
						}
					}
				}
			}
		}
	}
	return nullptr;
}
