// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorSceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "HighResScreenshot.h"
#include "ImageMagickBPLibrary.h"
#include "HaoshaSDKBPLibrary.h"
#include "GFileHelper.h"
#include "OSSHelper.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/Material.h"
#include "Slate/SceneViewport.h"
#include "HAL/FileManager.h"
#include "Modules/ModuleManager.h"
#include "UnrealClient.h"
#include "Misc/Paths.h"
#include "HAL/IConsoleManager.h"
#include "IOSSDownload.h"
#include "Misc/Paths.h"
#include "Engine/Engine.h"
#include "GameDirector.h"
#include "LogicDesignScene.h"

static TAutoConsoleVariable<int32> CVarSaveEXRCompressionQuality(
                                                                 TEXT("r.SaveEXR.CompressionQuality"),
                                                                 1,
                                                                 TEXT("Defines how we save HDR screenshots in the EXR format.\n")
                                                                 TEXT(" 0: no compression\n")
                                                                 TEXT(" 1: default compression which can be slow (default)"),
                                                                 ECVF_RenderThreadSafe);

FGighResScreenshotConfig& GetGHighResScreenshotConfig()
{
    static FGighResScreenshotConfig gInstance;
    return gInstance;
}

const float FGighResScreenshotConfig::MinResolutionMultipler = 1.0f;
const float FGighResScreenshotConfig::MaxResolutionMultipler = 10.0f;

FGighResScreenshotConfig::FGighResScreenshotConfig()
: ResolutionMultiplier(FGighResScreenshotConfig::MinResolutionMultipler)
, ResolutionMultiplierScale(0.0f)
, bMaskEnabled(false)
, bDumpBufferVisualizationTargets(false)
{
    ChangeViewport(TWeakPtr<FSceneViewport>());
    SetHDRCapture(false);
    SetForce128BitRendering(false);
    
    Init();
}

void FGighResScreenshotConfig::Init(uint32 NumAsyncWriters)
{
    IImageWrapperModule* ImageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>(FName("ImageWrapper"));
    if (ImageWrapperModule != nullptr)
    {
        ImageCompressorsLDR.Reserve(NumAsyncWriters);
        ImageCompressorsHDR.Reserve(NumAsyncWriters);
        
        for (uint32 Index = 0; Index != NumAsyncWriters; ++Index)
        {
            ImageCompressorsLDR.Emplace(ImageWrapperModule->CreateImageWrapper(EImageFormat::PNG));
            ImageCompressorsHDR.Emplace(ImageWrapperModule->CreateImageWrapper(EImageFormat::EXR));
        }
    }
    
#if WITH_EDITOR
    HighResScreenshotMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/EngineMaterials/HighResScreenshot.HighResScreenshot"));
    HighResScreenshotMaskMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/EngineMaterials/HighResScreenshotMask.HighResScreenshotMask"));
    HighResScreenshotCaptureRegionMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/EngineMaterials/HighResScreenshotCaptureRegion.HighResScreenshotCaptureRegion"));
    
    if (HighResScreenshotMaterial)
    {
        HighResScreenshotMaterial->AddToRoot();
    }
    if (HighResScreenshotMaskMaterial)
    {
        HighResScreenshotMaskMaterial->AddToRoot();
    }
    if (HighResScreenshotCaptureRegionMaterial)
    {
        HighResScreenshotCaptureRegionMaterial->AddToRoot();
    }
#endif
}

void FGighResScreenshotConfig::ChangeViewport(TWeakPtr<FSceneViewport> InViewport)
{
    if (FSceneViewport* Viewport = TargetViewport.Pin().Get())
    {
        // Force an invalidate on the old viewport to make sure we clear away the capture region effect
        Viewport->Invalidate();
    }
    
    UnscaledCaptureRegion = FIntRect(0, 0, 0, 0);
    CaptureRegion = UnscaledCaptureRegion;
    bMaskEnabled = false;
    bDumpBufferVisualizationTargets = false;
    ResolutionMultiplier = 1.0f;
    TargetViewport = InViewport;
}
/*
 bool FGighResScreenshotConfig::ParseConsoleCommand(const FString& InCmd, FOutputDevice& Ar)
 {
 GScreenshotResolutionX = 0;
 GScreenshotResolutionY = 0;
 ResolutionMultiplier = 1.0f;
 
 if( GetHighResScreenShotInput(*InCmd, Ar, GScreenshotResolutionX, GScreenshotResolutionY, ResolutionMultiplier, CaptureRegion, bMaskEnabled, bDumpBufferVisualizationTargets, bCaptureHDR) )
 {
 GScreenshotResolutionX *= ResolutionMultiplier;
 GScreenshotResolutionY *= ResolutionMultiplier;
 
 uint32 MaxTextureDimension = GetMax2DTextureDimension();
 
 // Check that we can actually create a destination texture of this size
 if ( GScreenshotResolutionX > MaxTextureDimension || GScreenshotResolutionY > MaxTextureDimension )
 {
 //Ar.Logf(TEXT("Error: Screenshot size exceeds the maximum allowed texture size (%d x %d)"), GetMax2DTextureDimension(), GetMax2DTextureDimension());
 return false;
 }
 
 GIsHighResScreenshot = true;
 
 return true;
 }
 
 return false;
 }
 */
bool FGighResScreenshotConfig::MergeMaskIntoAlpha(TArray<FColor>& InBitmap)
{
    bool bWritten = false;
    
    if (bMaskEnabled)
    {
        // If this is a high resolution screenshot and we are using the masking feature,
        // Get the results of the mask rendering pass and insert into the alpha channel of the screenshot.
        TArray<FColor>* MaskArray = FScreenshotRequest::GetHighresScreenshotMaskColorArray();
        check(MaskArray->Num() == InBitmap.Num());
        for (int32 i = 0; i < MaskArray->Num(); ++i)
        {
            InBitmap[i].A = (*MaskArray)[i].R;
        }
        
        bWritten = true;
    }
    else
    {
        // Ensure that all pixels' alpha is set to 255
        for (auto& Color : InBitmap)
        {
            Color.A = 255;
        }
    }
    
    return bWritten;
}

void FGighResScreenshotConfig::SetHDRCapture(bool bCaptureHDRIN)
{
    bCaptureHDR = bCaptureHDRIN;
}

void FGighResScreenshotConfig::SetForce128BitRendering(bool bForce)
{
    bForce128BitRendering = bForce;
}

bool FGighResScreenshotConfig::SetResolution(uint32 ResolutionX, uint32 ResolutionY, float ResolutionScale)
{
    if (ResolutionX > GetMax2DTextureDimension() || ResolutionY > GetMax2DTextureDimension())
    {
        // TODO LOG
        //Ar.Logf(TEXT("Error: Screenshot size exceeds the maximum allowed texture size (%d x %d)"), GetMax2DTextureDimension(), GetMax2DTextureDimension());
        return false;
    }
    
    UnscaledCaptureRegion = FIntRect(0, 0, 0, 0);
    CaptureRegion = UnscaledCaptureRegion;
    bMaskEnabled = false;
    
    GScreenshotResolutionX = ResolutionX;
    GScreenshotResolutionY = ResolutionY;
    GIsHighResScreenshot = true;
    
    return true;
}

template<typename> struct FPixelTypeTraits {};

template<> struct FPixelTypeTraits<FColor>
{
    static const ERGBFormat SourceChannelLayout = ERGBFormat::BGRA;
    
    static FORCEINLINE bool IsWritingHDRImage(const bool)
    {
        return false;
    }
};

template<> struct FPixelTypeTraits<FFloat16Color>
{
    static const ERGBFormat SourceChannelLayout = ERGBFormat::RGBA;
    
    static FORCEINLINE bool IsWritingHDRImage(const bool bCaptureHDR)
    {
        static const auto CVarDumpFramesAsHDR = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.BufferVisualizationDumpFramesAsHDR"));
        return bCaptureHDR || CVarDumpFramesAsHDR->GetValueOnAnyThread();
    }
};

template<> struct FPixelTypeTraits<FLinearColor>
{
    static const ERGBFormat SourceChannelLayout = ERGBFormat::RGBA;
    
    static FORCEINLINE bool IsWritingHDRImage(const bool bCaptureHDR)
    {
        static const auto CVarDumpFramesAsHDR = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.BufferVisualizationDumpFramesAsHDR"));
        return bCaptureHDR || CVarDumpFramesAsHDR->GetValueOnAnyThread();
    }
};

template<typename TPixelType>
bool FGighResScreenshotConfig::SaveImage(FString& File, TArray<TPixelType>& Bitmap, FIntPoint& BitmapSize, FString* OutFilename)
{
    typedef FPixelTypeTraits<TPixelType> Traits;
    
    static_assert(ARE_TYPES_EQUAL(TPixelType, FFloat16Color) || ARE_TYPES_EQUAL(TPixelType, FColor) || ARE_TYPES_EQUAL(TPixelType, FLinearColor), "Source format must be either FColor, FLinearColor or FFloat16Color");
    const int32 x = BitmapSize.X;
    const int32 y = BitmapSize.Y;
    check(Bitmap.Num() == x * y);
    
    const bool bIsWritingHDRImage = Traits::IsWritingHDRImage(bCaptureHDR);
    
    IFileManager* FileManager = &IFileManager::Get();
    const size_t BitsPerPixel = (sizeof(TPixelType) / 4) * 8;
    
    const FImageWriter* ImageWriter = nullptr;
    {
        const TArray<FImageWriter>& ArrayToUse = bIsWritingHDRImage ? ImageCompressorsHDR : ImageCompressorsLDR;
        
        // Find a free image writer to use. This can potentially be called on many threads at the same time
        for (;;)
        {
            for (const FImageWriter& Writer : ArrayToUse)
            {
                if (!Writer.bInUse.AtomicSet(true))
                {
                    ImageWriter = &Writer;
                    break;
                }
            }
            
            if (ImageWriter)
            {
                break;
            }
            
            FPlatformProcess::Sleep(0.001f);
        }
    }
    
    // here we require the input file name to have an extension
    FString NewExtension = bIsWritingHDRImage ? TEXT(".exr") : TEXT(".png");
    FString Filename = FPaths::GetBaseFilename(File, false) + NewExtension;
    
    if (OutFilename != nullptr)
    {
        *OutFilename = Filename;
    }
    
    bool bSuccess = false;
    
    if (ImageWriter != nullptr &&
        ImageWriter->ImageWrapper.IsValid() &&
        ImageWriter->ImageWrapper->SetRaw((void*)&Bitmap[0], sizeof(TPixelType)* x * y, x, y, Traits::SourceChannelLayout, BitsPerPixel))
    {
        EImageCompressionQuality LocalCompressionQuality = EImageCompressionQuality::Default;
        if (bIsWritingHDRImage && CVarSaveEXRCompressionQuality.GetValueOnAnyThread() == 0)
        {
            LocalCompressionQuality = EImageCompressionQuality::Uncompressed;
        }
        
        // Compress and write image
#if PLATFORM_IOS || PLATFORM_MAC
        // Compress and write image
        const TArray<uint8>& CompressedData = ImageWriter->ImageWrapper->GetCompressed(100);
        int32 CompressedSize = CompressedData.Num();
        
        NSData * mData = [NSData dataWithBytes : (void*)CompressedData.GetData() length : CompressedSize];
        
        NSString * mPath = [[NSString alloc]initWithUTF8String:TCHAR_TO_UTF8(*Filename)];
        
        [mData writeToFile : mPath atomically : YES];
        
        bSuccess = true;
        
#else
        // Compress and write image
        FArchive* Ar = FileManager->CreateFileWriter(Filename.GetCharArray().GetData());
        if (Ar != nullptr)
        {
            const TArray<uint8>& CompressedData = ImageWriter->ImageWrapper->GetCompressed(100);
            int32 CompressedSize = CompressedData.Num();
            Ar->Serialize((void*)CompressedData.GetData(), CompressedSize);
            delete Ar;
            
            bSuccess = true;
        }
#endif
    }
    
    ImageWriter->bInUse = false;
    
    return bSuccess;
}

template  bool FGighResScreenshotConfig::SaveImage<FColor>(FString& File, TArray<FColor>& Bitmap, FIntPoint& BitmapSize, FString* OutFilename);
template  bool FGighResScreenshotConfig::SaveImage<FFloat16Color>(FString& File, TArray<FFloat16Color>& Bitmap, FIntPoint& BitmapSize, FString* OutFilename);
template  bool FGighResScreenshotConfig::SaveImage<FLinearColor>(FString& File, TArray<FLinearColor>& Bitmap, FIntPoint& BitmapSize, FString* OutFilename);

FGighResScreenshotConfig::FImageWriter::FImageWriter(const TSharedPtr<class IImageWrapper>& InWrapper)
: ImageWrapper(InWrapper)
{
}


void AEditorSceneCapture2D::UploadUserStyleToOss()
{
    if (GPlatformFileManager::FileExists(*m_LocalPath_1))
    {
        FOssUploadQueueParameter inUploadParameters;
        
        //路径.
        TArray<FOssReqeustParameter> FilePaths;
        
        //Oss相对路径.
        m_OssPath_1 =FString("Styles/Icons/")+GetDateUrl(*m_LocalPath_1);
        FilePaths.Add(FOssReqeustParameter(m_OssPath_1, m_LocalPath_1));
        
        //完成回调,回调函数必须指明类.
        FOnOSSRequestFinishedDelegate OnOSSRequestFinished(this,&AEditorSceneCapture2D::OnUploadStyleIconToOssFinished);
        FOnOSSFileUploadProgressDelegate OnOSSFileUploadProgressDelegate(this, &AEditorSceneCapture2D::OnUploadStyleIconToOssProgress);
        
        
        inUploadParameters.OnOSSRequestFinished = OnOSSRequestFinished;
        inUploadParameters.OnOSSFileUploadProgress = OnOSSFileUploadProgressDelegate;
        inUploadParameters.FilePaths = FilePaths;
        //开始上传.
        OSSHelper::GetInstance()->GetOSSTool()->Upload(inUploadParameters);
    }
}


void AEditorSceneCapture2D::UploadUserStyle12ImageToOss()
{
    
}

bool AEditorSceneCapture2D::SaveRenderTargetToLocal(FString _LocalPath, bool isConvertJpg)
{
    bool mRet = false;
    UTextureRenderTarget2D* TextureTarget= GetCaptureComponent2D()->TextureTarget;
    
    if (TextureTarget==nullptr)
    {
        return mRet;
    }
    FTextureRenderTargetResource* RTResource = TextureTarget->GameThread_GetRenderTargetResource();
    
    FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
    ReadPixelFlags.SetLinearToGamma(true);
    
    TArray<FColor> OutBMP;
    RTResource->ReadPixels(OutBMP, ReadPixelFlags);
    
    for (FColor& color : OutBMP)
    {
        color.A = 255;
    }
    
    FIntRect SourceRect;
    
    FIntPoint DestSize(TextureTarget->GetSurfaceWidth(), TextureTarget->GetSurfaceHeight());
    
    
    FGighResScreenshotConfig& mGighResScreenshotConfig = GetGHighResScreenshotConfig();
    

    GPlatformFileManager::RecursiveCreateDirectory(*_LocalPath);

	UE_LOG(LogTemp, Log, TEXT("!!!_LocalPath：=%s"), *_LocalPath);

	_LocalPath = FPaths::ConvertRelativePathToFull(_LocalPath);

	UE_LOG(LogTemp, Log, TEXT("!!!_LocalPath1：=%s"), *_LocalPath);

    mRet = mGighResScreenshotConfig.SaveImage(_LocalPath, OutBMP, DestSize,NULL);
    
    FString mflipFilePath = _LocalPath;
    m_NativeAbsolutelyPaht = mflipFilePath;
    if (isConvertJpg)
    {
        mflipFilePath = UImageMagickBPLibrary::ConvertPngToJpg(_LocalPath);
        m_LocalPath_1 = mflipFilePath;
        m_NativeAbsolutelyPaht = mflipFilePath;
        GPlatformFileManager::DeleteFile(_LocalPath);
    }
#if PLATFORM_ANDROID
    UHaoshaSDKBPLibrary::FlipImage(mflipFilePath, mflipFilePath);
#endif
    
    return mRet;
}

FString AEditorSceneCapture2D::SetStyleIconLocalPath()
{
    m_LocalPath_1 = NULL;
    FGuid mGUID(0, 0, 0, 0);
    FPlatformMisc::CreateGuid(mGUID);
    FString mDir= GFileHelper::ProjectPersistentDownloadDir();
    m_LocalPath_1 = FString::Printf(TEXT("%s/styleIcon/%s.png"),*mDir,* mGUID.ToString());
    //m_LocalPath_1 = mDir + "/" + mGUID.ToString()+ ".png";
    
    GPlatformFileManager::RecursiveCreateDirectory(*m_LocalPath_1);
    
    return m_LocalPath_1;
}

void AEditorSceneCapture2D::OnUploadStyleIconToOssFinished(bool res)
{
	UE_LOG(LogTemp, Log, TEXT("!!!OnUploadStyleIconToOssFinished"));
    if (m_OssPath_1.Len()>0)
    {
        LogicDesignScene *mLogicDesignScene = (LogicDesignScene*)UGameDirector::GetInstance()->GetCurrentLogicScene();
        TArray<FString> mTemp;
        for (int32 i = 0; i < 12; i++)
        {
            mTemp.Add(m_OssPath_1);
        }
        //上传款JSON到服务器.
        mLogicDesignScene->SaveStyle(m_OssPath_1, mTemp);
        
        //GPlatformFileManager::DeleteFile(m_NativeAbsolutelyPaht);
    }
    
}

void AEditorSceneCapture2D::OnUploadStyleIconToOssProgress(FString _filename, int32 _currentsize, int32 _totalsize)
{
    
}

FString AEditorSceneCapture2D::GetDateUrl(const TCHAR * _inFilePath)
{
    FString mRet;
    FString fileName = GPlatformFileManager::GetFileName(_inFilePath);
    FString fileExt = GPlatformFileManager::GetFilePathExtPart(_inFilePath);
    mRet = GPlatformFileManager::CreateDateUrl() + "/" + fileName + "." + fileExt;
    return mRet;
}
