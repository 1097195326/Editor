#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Animation/AnimationAsset.h"
#include "Walker.h"
#include "System/Render/BaseActor.h"
#include "Catwalks.generated.h"


/** 走秀*/
UCLASS()
class EDITOR_API ACatwalks : public ABaseActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	ACatwalks();

protected:
	virtual void OnBeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Walk)
	void AddWalker(AWalker *inNewWalk);

	UFUNCTION(BlueprintCallable, Category = Walk)
	void Go();

	UFUNCTION(BlueprintCallable, Category = Walk)
	void ReleaseSource();

	UFUNCTION(BlueprintCallable, Category = Walk)
		void ReturnDesignLevel();

	void LoadAnimation(const FString & _animationPath);

	void LoadModel(const FString & _modelPath);
private:

	UPROPERTY(Transient)
	TArray<AWalker *> Walkers;

public:
	UPROPERTY()
	UAnimationAsset *WalkTargetAnim;


	UPROPERTY(BlueprintReadWrite, Category = Walk)
	USkeletalMeshComponent *BaseSkeletalMesh;
};
