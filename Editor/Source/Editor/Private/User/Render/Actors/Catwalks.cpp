#include "Catwalks.h"
#include "GameDirector.h"
#include "LogicDesignScene.h"
#include "MsgCenter.h"
#include "Editor.h"
#include "LoadAssetTool.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"

ACatwalks::ACatwalks(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	BaseSkeletalMesh(nullptr),
	WalkTargetAnim(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	BaseSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BASESKELETALMESH"));
	BaseSkeletalMesh->LightingChannels.bChannel2 = true;
	BaseSkeletalMesh->LightingChannels.bChannel1 = false;
	BaseSkeletalMesh->LightingChannels.bChannel0 = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BaseSkeletalMesh->SetupAttachment(RootComponent);
}

void ACatwalks::OnBeginPlay()
{

}

void ACatwalks::LoadAnimation(const FString & _animationPath)
{
	FString mPath = FString::Printf(TEXT("AnimSequence'/Game%s'"), *_animationPath);
	/*FStreamableManager AssetLoader;
	FStringAssetReference showOne(mPath);
	WalkTargetAnim = Cast<UAnimationAsset>(AssetLoader.LoadSynchronous(showOne));*/
	WalkTargetAnim = ULoadAssetTool::GetInstance()->LoadAsset<UAnimationAsset>(mPath);

	if (nullptr != WalkTargetAnim && WalkTargetAnim->IsValidLowLevel())
	{
		UE_LOG(GEditorLog, Error, TEXT("gx=== animation load success= %s"), *mPath);
	}
	else
	{
		UE_LOG(GEditorLog, Error, TEXT("gx=== animation load faild= %s"), *mPath);
	}
}

void ACatwalks::LoadModel(const FString & _modelPath)
{
	FString mPath = FString::Printf(TEXT("SkeletalMesh'/Game%s'"), *_modelPath);
	//BaseResource/Model/YellowWoman_01.YellowWoman_01
	/*FStreamableManager AssetLoader;
	FStringAssetReference yongZhongModel(mPath);
	BaseSkeletalMesh->SetSkeletalMesh(Cast<USkeletalMesh>(AssetLoader.LoadSynchronous(yongZhongModel)));*/
	BaseSkeletalMesh->SetSkeletalMesh(ULoadAssetTool::GetInstance()->LoadAsset<USkeletalMesh>(mPath));
	BaseSkeletalMesh->bEnablePerPolyCollision = true;
	BaseSkeletalMesh->bUseAttachParentBound = false;
	BaseSkeletalMesh->SetBoundsScale(10.0f);
	//BaseSkeletalMesh->SetBoundsScale(3.0f);
}


void ACatwalks::Tick(float DeltaTime)
{    
	Super::Tick(DeltaTime);
}

void ACatwalks::AddWalker(AWalker *inNewWalk)
{
	Walkers.AddUnique(inNewWalk);
	inNewWalk->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ACatwalks::Go()
{
	for (AWalker *walker : Walkers)
	{
		walker->GoWalk(BaseSkeletalMesh);
	}
	BaseSkeletalMesh->PlayAnimation(WalkTargetAnim, false);
}

void ACatwalks::ReleaseSource()
{
	for (AWalker *walker : Walkers)
	{
		if (walker->IsValidLowLevel() &&  !walker->IsPendingKill())
		{
			walker->Destroy();
		}
	}

	Walkers.Empty();
}

void ACatwalks::ReturnDesignLevel()
{
	// 发送返回设计场景的通知.
	msg_ptr mLocalMsg(new MsgLocal(MessageTypeLocal, LogicDesignScene::m_rebackDesignSceneMsg,nullptr));

	MsgCenter::Instance()->SendMsg(mLocalMsg);
}


