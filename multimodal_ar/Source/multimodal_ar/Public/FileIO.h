// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileIO.generated.h"

/**
 * 
 */
UCLASS()
class MULTIMODAL_AR_API UFileIO : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public: 

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static FString LoadFileToString(FString Filename);
	
	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static TArray<FString> LoadFileToStringArray(FString Filename);

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static FString GetGameSourcePath();

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static FString GetGeneratedConfigPath();

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool WriteStringToProjectConfigFile(const FString Section, const FString FileName, const FString Key, const FString Value);
	
	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool ReadStringFromProjectConfigFile(const FString Section, const FString FileName, const FString Key, FString &Value);

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool WriteStringToGeneratedConfigFile(const FString Section, const FString FileName, const FString Key, const FString Value);
	
	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool ReadStringFromGeneratedConfigFile(const FString Section, const FString FileName, const FString Key, FString& Value);

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool GetSectionArrayFromProjectConfigFile(const FString Section, const FString FileName, TArray<FString>& Result);

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool GetSectionMapFromProjectConfigFile(const FString Section, const FString FileName, TMap<FString, FString>& Result);

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool GetSectionArrayFromGeneratedConfigFile(const FString Section, const FString FileName, TArray<FString>& Result);

	UFUNCTION(BlueprintCallable, Category = "File I/O")
	static bool GetSectionMapFromGeneratedConfigFile(const FString Section, const FString FileName, TMap<FString, FString>& Result);


};
