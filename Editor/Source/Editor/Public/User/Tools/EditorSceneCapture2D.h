// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "Runtime/RHI/Public/RHI.h"
#include "EditorSceneCapture2D.generated.h"

/**
*
*/
class FSceneViewport;
class IImageWrapper;
class UMaterial;


struct FGighResScreenshotConfig
{
	static const float MinResolutionMultipler;
	static const float MaxResolutionMultipler;

	FIntRect UnscaledCaptureRegion;
	FIntRect CaptureRegion;
	float ResolutionMultiplier;
	float ResolutionMultiplierScale;
	bool bMaskEnabled;
	bool bDumpBufferVisualizationTargets;
	TWeakPtr<FSceneViewport> TargetViewport;
	bool bDisplayCaptureRegion;
	bool bCaptureHDR;
	bool bForce128BitRendering;

	// Materials used in the editor to help with the capture of highres screenshots
	UMaterial* HighResScreenshotMaterial;
	UMaterial* HighResScreenshotMaskMaterial;
	UMaterial* HighResScreenshotCaptureRegionMaterial;

	FGighResScreenshotConfig();

	/** Initialize the image wrapper modules (required for SaveImage) **/
	void Init(uint32 NumAsyncWriters = 6);

	/** Point the screenshot UI at a different viewport **/
	void ChangeViewport(TWeakPtr<FSceneViewport> InViewport);

	/** Parse screenshot parameters from the supplied console command line **/
	//bool ParseConsoleCommand(const FString& InCmd, FOutputDevice& Ar);

	/** Utility function for merging the mask buffer into the alpha channel of the supplied bitmap, if masking is enabled.
	* Returns true if the mask was written, and false otherwise.
	**/
	bool MergeMaskIntoAlpha(TArray<FColor>& InBitmap);

	/** Enable/disable HDR capable captures **/
	void SetHDRCapture(bool bCaptureHDRIN);

	/** Enable/disable forcing 128-bit rendering pipeline for capture **/
	void SetForce128BitRendering(bool bForce);

	/** Configure taking a high res screenshot */
	bool SetResolution(uint32 ResolutionX, uint32 ResolutionY, float ResolutionScale = 1.0f);

	/** Save to image file **/
	template<typename TPixelType>
	bool SaveImage(FString& File, TArray<TPixelType>& Bitmap, FIntPoint& BitmapSize, FString* OutFilename = nullptr);


private:
	struct FImageWriter
	{
		FImageWriter(const TSharedPtr<class IImageWrapper>& InWrapper);

		TSharedPtr<class IImageWrapper> ImageWrapper;
		mutable FThreadSafeBool bInUse;
	};
	TArray<FImageWriter> ImageCompressorsLDR;
	TArray<FImageWriter> ImageCompressorsHDR;
};

FGighResScreenshotConfig& GetGHighResScreenshotConfig();

UCLASS()
class EDITOR_API AEditorSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()

	//上传用户款ICON到OSS.
	UFUNCTION(BlueprintCallable)
	void UploadUserStyleToOss();

	//上传用户12张图到OSS.
	UFUNCTION(BlueprintCallable)
	void UploadUserStyle12ImageToOss();

	//保存RT到本地.
	UFUNCTION(BlueprintCallable)
	bool SaveRenderTargetToLocal(FString _LocalPath, bool isConvertJpg);

	//获取本地路径.
	UFUNCTION(BlueprintCallable)
	FString SetStyleIconLocalPath();

	void OnUploadStyleIconToOssFinished(bool res = true);
	void OnUploadStyleIconToOssProgress(FString _filename, int32 _currentsize, int32 _totalsize);
	FString GetDateUrl(const TCHAR * _inFilePath);
	void Split_ColorArray_8bpp(const TArray<FColor>& DecompressedImageRGBA, uint8* DecompressedImageA);
public:
	//1本地路径.
	UPROPERTY(BlueprintReadWrite)
		FString m_LocalPath_1;
	//1OSS路径.  
	UPROPERTY(BlueprintReadWrite)
		FString m_OssPath_1;
	//12本地路径.
	UPROPERTY(BlueprintReadWrite)
		TArray<FString> m_LocalPath_12;
	//12OSS路径.
	UPROPERTY(BlueprintReadWrite)
		TArray<FString> m_OssPath_12;

	FString m_NativeAbsolutelyPaht;


};
