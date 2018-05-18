// Fill out your copyright notice in the Description page of Project Settings.
#include "DesignActorRender.h"
#include "ComponentStruct.h"
#include "DesignModule.h"
#include "StyleComponent.h"
#include "GModuleManager.h"
#include "GPathTool.h"
#include "BaseComponent.h"
#include "IOSSDownload.h"
#include "OSSHelper.h"
#include "GPlatformMgr.h"
#include "DownloadImageTool.h"
#include "UIManager.h"
#include "DesignStaticActor.h"
#include "SuitT.h"
#include "FabricComponent.h"
#include "Catwalks.h"
#include "GPlatformFileManager.h"
#include "LogicDesignScene.h"
#include "StyleComponent.h"
#include "GLanguageTool.h"
#include "GameDirector.h"
#include "GResourceManager.h"
#include "GFileHelper.h"

DesignActorRender::DesignActorRender()
{

}

DesignActorRender::~DesignActorRender()
{

}

void DesignActorRender::OnLoad()
{
	DesignModule * mDesignModule = GModuleManager::GetInstance()->GetModule<DesignModule>("DesignModule");

	int32 mRid = mDesignModule->GetDesignData()->m_designRID;

	int32 mTid = mDesignModule->GetDesignData()->m_designTID;

	// 创建Component.
	m_rootComponent = new StyleComponent();

	((BaseComponent*)m_rootComponent)->GLoad(this);

	m_rootComponent->InitData(mRid, mTid, 0,nullptr);
}

void DesignActorRender::OnRelease()
{
	m_walker = nullptr;
}

void DesignActorRender::StartRender()
{
	FString mLanguage = GLanguageTool::GetInstance()->GetLanguage(TEXT("key133"));

	UUIManager::GetInstance()->OpenMiddleTip(mLanguage, 0.0f);

	// 1.先判断下载花型.
	DownloadPattern();
}

void DesignActorRender::StartSkeletalRender()
{
	RemoveRender();

	m_isFirstRender = true;

	// 创建actor.
	if (nullptr == m_walker)
	{
		m_walker = UGameDirector::GetInstance()->GetWorld()->SpawnActor<AWalker>(AWalker::StaticClass());
		m_walker->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	ACatwalks * mCatwalks = nullptr;
	if (nullptr == m_actor)
	{
		mCatwalks = UGResourceManager::GetInstance()->LoadAssetDontCache<ACatwalks>(GetCatWalkActorPath());
		mCatwalks->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));

		m_actor = mCatwalks;
	}

	// 创建所有的component模型.
	BaseComponent * mComponent = (BaseComponent*)m_rootComponent;
	
	// 开始渲染.
	mCatwalks->ReleaseSource();

	mComponent->StartWalkingRender();

	mCatwalks->AddWalker(m_walker);

	mCatwalks->Go();
}

void DesignActorRender::RemoveRender()
{
	if (nullptr != m_actor)
	{
		m_rootComponent->ReleaseRender();

		m_actor = nullptr;
	}

	if (nullptr != m_walker)
	{
		m_rootComponent->ReleaseRender();

		m_walker = nullptr;
	}
}

AWalker * DesignActorRender::GetPawn()
{
	return m_walker;
}

void DesignActorRender::GetAllPattern(TArray<FString> & _pattenPathArr)
{
	BaseComponent * mComponent = (BaseComponent*)m_rootComponent;

	mComponent->GetAllTexturePath(_pattenPathArr);

	mComponent->GetAllColorTexture(_pattenPathArr);
}

void DesignActorRender::GetAllPatternIds(TArray<FString> & _pattenPathArr)
{
	BaseComponent * mComponent = (BaseComponent*)m_rootComponent;

	mComponent->GetAllPatternIds(_pattenPathArr);
}

void DesignActorRender::DownloadPattern()
{
	TArray<FString> mPatternArray;

	GetAllPattern(mPatternArray);

	if (mPatternArray.Num() == 0)
	{
		StartRenderOnCheckPattern();
	}

	TArray<DownloadImageParam*> m_params;

	// 判断路径并OSS下载.
	for (int32 i = 0; i < mPatternArray.Num(); i++)
	{
		// 先判断一下本地是否存在图片.
		FString mPatternPath = mPatternArray[i];

		FString mSavePath = GPathTool::GetHttpPath(mPatternPath);
		
		bool mCheck = GPlatformFileManager::FileExists(*mSavePath);

		if (!mCheck)
		{
			// 创建目录.
			GPlatformFileManager::RecursiveCreateDirectory(*mSavePath);

			bool mIsHave = false;

			for (int32 j = 0; j < m_params.Num(); j++)
			{
				DownloadImageParam * mParameter = m_params[j];

				if (mParameter->m_downloadUrl.Equals(mPatternPath))
				{
					mIsHave = true;
					break;
				}
			}
			if (!mIsHave)
			{
				m_params.Add(new DownloadImageParam(mPatternPath, mSavePath));
			}
		}
	}

	if (m_params.Num() > 0)
	{
		//完成回调.
		DownloadImageStruct * mDownloadStruct = new DownloadImageStruct(m_params,this,&DesignActorRender::DownloadPatternFinish);

		UDownloadImageTool::GetInstance()->DownloadImage(mDownloadStruct);
	}
	else
	{
		StartRenderOnCheckPattern();
	}
}

void DesignActorRender::StartRenderOnCheckPattern()
{
	if (GetTextureManager()->ResourceCount() > 3)
	{
		GetTextureManager()->Clear();
	}
	
	// 创建actor.
	if (nullptr == m_actor)
	{
		m_actor = UGResourceManager::GetInstance()->LoadAssetDontCache<ADesignStaticActor>(GetTableActorPath());
		m_actor->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	((ADesignStaticActor*)m_actor)->InitTable();
	
	// 创建所有的component模型.
	BaseComponent * mComponent = (BaseComponent*)m_rootComponent;

	mComponent->StartDesignRender();

	UUIManager::GetInstance()->CloseMiddleTip();

	// 发送通知.
	msg_ptr mLocalEvent(new MsgLocal(MessageTypeLocal,3020,nullptr));

	MsgCenter::Instance()->SendMsg(mLocalEvent);

	// 通知场景已经渲染完成.
	if (m_isFirstRender)
	{
		LogicDesignScene * mDesignScene = (LogicDesignScene *)UGameDirector::GetInstance()->GetCurrentLogicScene();

		mDesignScene->OnRenderFinish();

		m_isFirstRender = false;
	}
}

void DesignActorRender::DownloadPatternProcess(FString, int32, int32)
{

}

void DesignActorRender::DownloadPatternFinish()
{
	StartRenderOnCheckPattern();
}

FString DesignActorRender::GetCatWalkActorPath()
{
	StyleComponent* mStyleComponent = (StyleComponent*)m_rootComponent;

	SuitT * mSuitT = mStyleComponent->GetDataC()->GetBaseT<SuitT>();
	
	FString mActorPath = GPathTool::GetWalkActorPath(mSuitT->m_walkSequence);

	return FString::Printf(TEXT("/Game%s_C"), *mActorPath);
}

FString DesignActorRender::GetTableActorPath()
{
	StyleComponent* mStyleComponent = (StyleComponent*)m_rootComponent;

	SuitT * mSuitT = mStyleComponent->GetDataC()->GetBaseT<SuitT>();

	FString mActorPath = GPathTool::GetTableActorPath(mSuitT->m_tableModel);

	return FString::Printf(TEXT("/Game%s_C"), *mActorPath);
}
EnumComponentType DesignActorRender::GetComponentTypeByFabricId(int32 _fabricId)
{
	TArray<IComponent*> _childComponentArray;

	m_rootComponent->GetComponentByTag(EnumComponentType::FABRIC_COMPONENT, _childComponentArray);

	for (int32 i = 0; i < _childComponentArray.Num(); i++)
	{
		FabricComponent * mFabricComponent = (FabricComponent *)_childComponentArray[i];

		if (mFabricComponent->GetDataC()->GetBaseT()->m_Tid == _fabricId)
		{
			// 面料上一级= 片 片的上一级=部件 部件的上一级等于部位.
			return (EnumComponentType)(mFabricComponent->GetParentComponent()->GetParentComponent()->GetParentComponent()->GetComponentTag());
		}
	}

	return EnumComponentType::PART_COMPONENT;
}