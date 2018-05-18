// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "RunTime/UMG/Public/Blueprint/AsyncTaskDownloadImage.h"
#include "LoadAssetTool.h"


static void GWriteRawToTexture_RenderThread(FTexture2DDynamicResource* TextureResource, const TArray<uint8>& RawData, bool bUseSRGB = true)
{
	check(IsInRenderingThread());

	FTexture2DRHIParamRef TextureRHI = TextureResource->GetTexture2DRHI();

	int32 Width = TextureRHI->GetSizeX();
	int32 Height = TextureRHI->GetSizeY();

	uint32 DestStride = 0;
	uint8* DestData = reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_WriteOnly, DestStride, false, false));

	for (int32 y = 0; y < Height; y++)
	{
		uint8* DestPtr = &DestData[(Height - 1 - y) * DestStride];

		const FColor* SrcPtr = &((FColor*)(RawData.GetData()))[(Height - 1 - y) * Width];
		for (int32 x = 0; x < Width; x++)
		{
			*DestPtr++ = SrcPtr->B;
			*DestPtr++ = SrcPtr->G;
			*DestPtr++ = SrcPtr->R;
			*DestPtr++ = SrcPtr->A;
			SrcPtr++;
		}
	}

	RHIUnlockTexture2D(TextureRHI, 0, false, false);
}

ULoadAssetTool * ULoadAssetTool::m_pInstance = nullptr;

ULoadAssetTool::ULoadAssetTool()
{

}

ULoadAssetTool::~ULoadAssetTool()
{

}

ULoadAssetTool * ULoadAssetTool::GetInstance()
{
	if (m_pInstance == nullptr)  //判断是否第一次调用.
	{
		m_pInstance = NewObject<ULoadAssetTool>();

		m_pInstance->AddToRoot();
	}
	return m_pInstance;
}

void ULoadAssetTool::GLoad()
{

}

void ULoadAssetTool::GRelease()
{
	m_pInstance->RemoveFromRoot();

	m_pInstance = nullptr;
}

FString ULoadAssetTool::FormatUrlToUE4AssertFilePath(const FString &_inUrlPath) const
{
	FString urlPath = _inUrlPath;
	FString fileName = FPaths::GetBaseFilename(_inUrlPath);
	FString right;
	if (_inUrlPath.Split(".", nullptr, &right, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		return urlPath.Replace(*right, *fileName);
	}
	else
	{
		//UE_LOG(ClothEditor, Error, TEXT("ERROR ASSERT URL PATH %s"), *_inUrlPath);
	}
	return FString();
}


UTexture2DDynamic * ULoadAssetTool::CreateTextureByByte(uint8* _array, int32 _lenth)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrappers[5] =
	{
		ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO),
		ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS),
	};

	for (auto ImageWrapper : ImageWrappers)
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(_array, _lenth))
		{
			const TArray<uint8>* RawData = NULL;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
			{
				if (UTexture2DDynamic* Texture = UTexture2DDynamic::Create(ImageWrapper->GetWidth(), ImageWrapper->GetHeight()))
				{
					Texture->SRGB = true;
					Texture->UpdateResource();

					struct FUpdateTextureRegionsData
					{
						FTexture2DResource* Texture2DResource;
						int32 MipIndex;
						uint32 NumRegions;
						FUpdateTextureRegion2D* Regions;
						uint32 SrcPitch;
						uint32 SrcBpp;
						uint8* SrcData;
					};

					ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
						FUpdateTextureRegionsData,
						FTexture2DDynamicResource*, TextureResource, static_cast<FTexture2DDynamicResource*>(Texture->Resource),
						TArray<uint8>, RawData, *RawData,
						{
							GWriteRawToTexture_RenderThread(TextureResource, RawData);
						});


					return Texture;
				}
			}
		}
	}
	return nullptr;
}

UClass* ULoadAssetTool::LoadUClass(const FString & _assetPath)
{
	return LoadClass<AActor>(NULL, *_assetPath);
}

void ULoadAssetTool::UnLoadAsset(FStringAssetReference & AssetToLoad)
{
	UGameDirector::GetInstance()->StreamableManager().Unload(AssetToLoad);
}

TSharedPtr<IImageWrapper> ULoadAssetTool::GetImageWrapperByExtention(const FString &_inImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	FString lowStr = _inImagePath.ToLower();
	if (lowStr.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		//UE_LOG(ClothEditor, Warning, TEXT("Test png GetImageWrapperByExtention   %s"), *InImagePath);
	}
	else if (lowStr.EndsWith(".jpg") || lowStr.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (lowStr.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (lowStr.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (lowStr.EndsWith(".exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (lowStr.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}
	return nullptr;
}
