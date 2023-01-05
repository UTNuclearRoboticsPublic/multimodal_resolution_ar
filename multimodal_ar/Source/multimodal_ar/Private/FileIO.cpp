// Fill out your copyright notice in the Description page of Project Settings.


#include "FileIO.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Misc/ConfigCacheIni.h"

/** Editor Only*/
FString UFileIO::LoadFileToString(FString Filename)
{
	FString Directory = FPaths::GameSourceDir();
	FString Result;
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();

	if (File.CreateDirectory(*Directory))
	{
		FString FullPath = Directory + "/" + Filename;
		FFileHelper::LoadFileToString(Result, *FullPath);
	}

	return Result;
}

/** Editor Only*/
TArray<FString> UFileIO::LoadFileToStringArray(FString Filename)
{
	FString Directory = FPaths::GameSourceDir();
	TArray<FString> Result;
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();
	FVector succ = { 100.f, 0.f, 0.f };
	
	if (File.CreateDirectory(*Directory))
	{
		FString FullPath = Directory + "/" + Filename;
		FFileHelper::LoadFileToStringArray(Result, *FullPath);
	}
	return Result;
}

FString UFileIO::GetGameSourcePath()
{
	return FPaths::GameSourceDir();
}

FString UFileIO::GetGeneratedConfigPath()
{
	return FPaths::GeneratedConfigDir();
}

bool UFileIO::WriteStringToProjectConfigFile(const FString Section, const FString FileName, const FString Key,  const FString Value)
{
	if (!GConfig) return false;

	FString FullFilePath = FPaths::ProjectConfigDir().Append(*FileName).Append(".ini");

	GConfig->SetString(*Section, *Key, *Value, FullFilePath);

	GConfig->Flush(false, FullFilePath);

	return true;
	
}

bool UFileIO::ReadStringFromProjectConfigFile(const FString Section, const FString FileName, const FString Key,  FString& Value)
{
	if (!GConfig) return false;

	FString FullFilePath = FPaths::ProjectConfigDir().Append(*FileName).Append(".ini");

	return GConfig->GetString(*Section, *Key, Value, FullFilePath);

}

bool UFileIO::WriteStringToGeneratedConfigFile(const FString Section, const FString FileName, const FString Key,  const FString Value)
{
	if (!GConfig) return false;

	FString FullFilePath = FPaths::GeneratedConfigDir().Append(*FileName).Append(".ini");

	GConfig->SetString(*Section, *Key, *Value, FullFilePath);

	GConfig->Flush(false, FullFilePath);

	return true;

}

bool UFileIO::ReadStringFromGeneratedConfigFile(const FString Section, const FString FileName, const FString Key,  FString& Value)
{
	if (!GConfig) return false;

	FString FullFilePath = FPaths::GeneratedConfigDir().Append(*FileName).Append(".ini");

	return GConfig->GetString(*Section, *Key, Value, FullFilePath);

}


bool UFileIO::GetSectionArrayFromProjectConfigFile(const FString Section, const FString FileName, TArray<FString>& Result)
{
	if (!GConfig) return false;
	
	FString FullFilePath = FPaths::ProjectConfigDir().Append(*FileName).Append(".ini");

	return GConfig->GetSection(*Section, Result, FullFilePath);
}

/** This Function attempts to convert the section string array into a map. If the Key string has a '=' character, this will result in incorrect parsing. */
bool UFileIO::GetSectionMapFromProjectConfigFile(const FString Section, const FString FileName, TMap<FString, FString>& Result)
{
	if (!GConfig) return false;

	FString FullFilePath = FPaths::ProjectConfigDir().Append(*FileName).Append(".ini");

	TArray<FString> SectionArray;
	GConfig->GetSection(*Section, SectionArray, FullFilePath);

	for (const auto KeyValuePair : SectionArray)
	{
		if (int location = KeyValuePair.Find("="))
		{
			Result.Add(KeyValuePair.Left(location), KeyValuePair.RightChop(location+1));
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

bool UFileIO::GetSectionArrayFromGeneratedConfigFile(const FString Section, const FString FileName, TArray<FString>& Result)
{
	if (!GConfig) return false;

	FString FullFilePath = FPaths::GeneratedConfigDir().Append(*FileName).Append(".ini");

	return GConfig->GetSection(*Section, Result, FullFilePath);
}

bool UFileIO::GetSectionMapFromGeneratedConfigFile(const FString Section, const FString FileName, TMap<FString, FString>& Result)
{
	if (!GConfig) return false;

	FString FullFilePath = FPaths::GeneratedConfigDir().Append(*FileName).Append(".ini");

	TArray<FString> SectionArray;
	GConfig->GetSection(*Section, SectionArray, FullFilePath);

	for (const auto KeyValuePair : SectionArray)
	{
		if (int location = KeyValuePair.Find("="))
		{
			Result.Add(KeyValuePair.Left(location), KeyValuePair.RightChop(location + 1));
		}
		else
		{
			return false;
		}
	}

	return true;
}
