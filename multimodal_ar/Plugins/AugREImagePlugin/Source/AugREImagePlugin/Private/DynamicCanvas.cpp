// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicCanvas.h"

void UDynamicCanvas::InitializeCanvas(const int32 pixelsH, const int32 pixelsV)
{
    //dynamic texture initialization
    CanvasWidth = pixelsH;
    CanvasHeight = pixelsV;
    BitDepth = 4;
    Pitch = CanvasWidth * BitDepth;
    // the number of pixels * bit depth (BGRX) - this is for reserving memory
    CanvasSize = CanvasWidth * CanvasHeight * BitDepth;

    DynamicCanvas = UTexture2D::CreateTransient(CanvasWidth, CanvasHeight, PF_B8G8R8A8);
    DynamicCanvas->AddToRoot();
    DynamicCanvas->UpdateResource();
    ClearCanvas();
}

void UDynamicCanvas::DecompressImage(const TArray<uint8>& SourceImageData) {
    IImageWrapperModule& imageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> imageWrapper = imageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);

    if (imageWrapper.IsValid() && imageWrapper->SetCompressed(SourceImageData.GetData(), SourceImageData.Num()))
    {
        TArray<uint8>& uncompressedBGRA = Buffer;
        if (imageWrapper->GetRaw(ERGBFormat::BGRA, 8, uncompressedBGRA))
        {
            if (!bIsBufferAllocated)
            {
                int32 height = imageWrapper->GetHeight();
                int32 width = imageWrapper->GetWidth();
                Buffer.SetNum(height * width * BitDepth);
                UE_LOG(LogTemp, Log, TEXT("buffer allocated to : %p with %d elements"), Buffer.GetData(), Buffer.Num());
                UE_LOG(LogTemp, Log, TEXT("image width: %d, height: %d"), width, height);
                bIsBufferAllocated = true;
            }

            UpdateCanvas(uncompressedBGRA);
        }
    }
}

void UDynamicCanvas::UpdateCanvas(const TArray<uint8>& NewImageData)
{
    
    FTexture2DMipMap* MipMap = &DynamicCanvas->PlatformData->Mips[0];
    FByteBulkData* ImageData = &MipMap->BulkData;
    uint8* RawImageData = (uint8*)ImageData->Lock(LOCK_READ_WRITE);
    
    //TODO: may be more efficient method to try, like memcpy or move
    for (auto i = 0; i < CanvasSize; i++)
    {
        RawImageData[i] = NewImageData[i];
    }

    ImageData->Unlock();
    DynamicCanvas->UpdateResource();
}

void UDynamicCanvas::ClearCanvas()
{
    FTexture2DMipMap* MipMap = &DynamicCanvas->PlatformData->Mips[0];
    FByteBulkData* ImageData = &MipMap->BulkData;
    uint8* RawImageData = (uint8*)ImageData->Lock(LOCK_READ_WRITE);

    for (auto i = 0; i < CanvasSize; i++) {
        RawImageData[i] = 255; // white (R:255, G:255, B:255, A:255)
    }

    ImageData->Unlock();
    DynamicCanvas->UpdateResource();
}

UDynamicCanvas::UDynamicCanvas()
{
}

UDynamicCanvas::~UDynamicCanvas()
{
}