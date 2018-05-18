// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "UITextureManager.generated.h"

USTRUCT()
struct FModelTextureStruct
{
	GENERATED_BODY()
public:
	// 资源GUID.
	UPROPERTY()
	int32 m_useCount = 0;

	UPROPERTY()
	FVector2D m_dpi;

	UPROPERTY()
	FVector2D m_textureSize;

	UPROPERTY()
	UTexture2D * m_texture2d = nullptr;

	FModelTextureStruct() {}
};

/**
 * 
 */
UCLASS()
class GCORE_API UUITextureManager : public UObject, public ILoad
{
	GENERATED_BODY()
	
private:

	// 一般是UI用, key是路径，value是值.
	UPROPERTY()
	TMap<FString, UTexture2D*>          m_resourceMap;

	UPROPERTY()
	TMap<FString, UTexture2D*>          m_contentResourceMap;

	// 场景模型贴图用.
	TMap<FString, FModelTextureStruct*>          m_resourceMapForModel;
	
public:
	UUITextureManager();

	virtual ~UUITextureManager();

	virtual void GLoad();

	virtual void GRelease();

	void Clear();

	int32 ResourceCount();

	
public: // UIController用.
	// 添加资源.
	void AddTexture2D(const FString & _key,UTexture2D * _texture2D);

	void AddContentTexture2D(const FString & _key, UTexture2D * _texture2D);

	UTexture2D * GetTexture2d(const FString & _key);

	UTexture2D * GetTexture2dByPath(const FString & _path);

	UTexture2D * GetTexture2dInContent(const FString & _path);

public: // 场景模型贴图缓存时候用.
	// 添加资源.
	void AddTexture2DForModel(const FString & _key, FModelTextureStruct * _modelTextureStruct);

	FModelTextureStruct * GetTexture2dForModel(const FString & _key);
};
