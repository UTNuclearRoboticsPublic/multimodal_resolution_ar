// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "UObject/Object.h"
#include <vector>
#include <memory>
#include <iterator>
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "DynamicCanvas.generated.h"

//Define Log Category and Verbosity
DECLARE_LOG_CATEGORY_EXTERN(LogDynamicCanvas, Log, All);

UCLASS(Blueprintable, BlueprintType)
class AUGREIMAGEPLUGIN_API UDynamicCanvas : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	UTexture2D* DynamicCanvas;

	UFUNCTION(BlueprintCallable, Category = DrawingTools)
	void InitializeCanvas(const int32 pixelsH, const int32 pixelsV);

	UFUNCTION(BlueprintCallable, Category = DrawingTools)
	void DecompressImage(const TArray<uint8>& SourceImageData);

	//UFUNCTION(BlueprintCallable, Category = DrawingTools)
	void UpdateCanvas(const TArray<uint8>& NewImageData);

	UFUNCTION(BlueprintCallable, Category = DrawingTools)
	void ClearCanvas();

	UDynamicCanvas();
	~UDynamicCanvas();

private:

	int CanvasWidth;
	int CanvasHeight;
	int BitDepth;
	int Pitch;
	int CanvasSize;
	bool bIsCompressed = false;
	bool bIsBufferAllocated = false;
	TArray<uint8> Buffer;
};
