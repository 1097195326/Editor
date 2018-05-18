// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILoad.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "GameDirector.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "LoadAssetTool.generated.h"

/**
 * 
 */
UCLASS()
class GCORE_API ULoadAssetTool : public UObject, public ILoad
{
	GENERATED_BODY()

private:
	// 单利.
	static ULoadAssetTool *				   m_pInstance;
	
public:
	ULoadAssetTool();

	~ULoadAssetTool();

	static ULoadAssetTool * GetInstance();

	virtual void GLoad() override;

	virtual void GRelease() override;

	TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString &_inImagePath);

	FString FormatUrlToUE4AssertFilePath(const FString &_inUrlPath) const;

	// 通过二进制创建纹理.
	UTexture2DDynamic * CreateTextureByByte(uint8* _array, int32 _lenth);

	// 创建模型时候用.
	UClass* LoadUClass(const FString & _assetPath);
	
public:
	template<typename T>
	T* LoadAsset(const FString & _objPath)
	{
		if (!_objPath.IsEmpty())
		{
			/*UObject* loadClass = StaticLoadObject(T::StaticClass(), nullptr, *_objPath);

			return Cast<T>(loadClass);*/
			//FStringAssetReference AssetToLoad(_objPath);

			//return Cast<T>(UGameDirector::GetInstance()->StreamableManager().LoadSynchronous(AssetToLoad));
			return LoadObject<T>(UGameDirector::GetInstance()->GetWorld(), *_objPath, *_objPath);
			//return (T*)StaticLoadObject(T::StaticClass(), nullptr, *_objPath, *_objPath, ELoadFlags::LOAD_None,nullptr, false);
			//return (T*)StaticLoadObject(T::StaticClass(), UGameDirector::GetInstance()->GetWorld(), *_objPath);
		}
		return nullptr;
	}

	template<typename T>
	T* LoadAssetByAssetRef(FStringAssetReference & AssetToLoad)
	{
		return UGameDirector::GetInstance()->StreamableManager().LoadSynchronous<T>(AssetToLoad);
	}

	void UnLoadAsset(FStringAssetReference & AssetToLoad);

	template<typename T>
	T* LoadUAsset(const FString &_inSourcePath)
	{
		FString assetPath;
		FString prefix;
		FString subfix(TEXT("'"));
		if (UTexture2D::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			//是虚幻支持的图片.直接加载!.
			TSharedPtr<IImageWrapper> imgWrapper = GetImageWrapperByExtention(_inSourcePath);
			if (imgWrapper.IsValid())
			{
				return LoadAsset<T>(_inSourcePath);
			}
			prefix = FString(TEXT("Texture2D'"));
		}
		else if (UStaticMesh::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			prefix = FString(TEXT("StaticMesh'"));
		}
		else if (USkeletalMesh::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			prefix = FString(TEXT("SkeletalMesh'"));
		}
		else if (UMaterialInterface::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			prefix = FString(TEXT("MaterialInstanceConstant'"));
		}
		else  
		{
			return nullptr;
		}
		FString urlPath = FormatUrlToUE4AssertFilePath(_inSourcePath);
		if (urlPath.Len() > 0)
		{
			assetPath = FPaths::Combine(prefix, TEXT("/Game"), urlPath) + subfix;

			return LoadAsset<T>(assetPath);
		}
		return nullptr;
	}

	template<typename T>
	FString GetAssetPath(const FString &_inSourcePath)
	{
		FString assetPath;
		FString prefix;
		FString subfix(TEXT("'"));
		if (UTexture2D::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			//是虚幻支持的图片.直接加载!.
			TSharedPtr<IImageWrapper> imgWrapper = GetImageWrapperByExtention(_inSourcePath);
			if (imgWrapper.IsValid())
			{
				return _inSourcePath;
			}
			prefix = FString(TEXT("Texture2D'"));
		}
		else if (UStaticMesh::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			prefix = FString(TEXT("StaticMesh'"));
		}
		else if (USkeletalMesh::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			prefix = FString(TEXT("SkeletalMesh'"));
		}
		else if (UMaterialInterface::StaticClass()->GetFName() == T::StaticClass()->GetFName())
		{
			prefix = FString(TEXT("MaterialInstanceConstant'"));
		}
		else
		{
			return _inSourcePath;
		}
		FString urlPath = FormatUrlToUE4AssertFilePath(_inSourcePath);
		if (urlPath.Len() > 0)
		{
			return FPaths::Combine(prefix, TEXT("/Game"), urlPath) + subfix;
		}
		return _inSourcePath;
	}

};
