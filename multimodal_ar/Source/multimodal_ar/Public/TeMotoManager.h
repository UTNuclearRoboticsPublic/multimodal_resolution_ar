// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobofleetUnrealClientModule.h"
#include "RobofleetClientBase.h"
#include "TeMotoManager.generated.h"

UCLASS()
class MULTIMODAL_AR_API ATeMotoManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere)
	TArray<FString> root_actions;

	//UPROPERTY(EditAnyWhere)
	std::unordered_multimap<std::string, std::string> graph;
	std::map<FString, FString> description;
	
public:	
	// Sets default values for this actor's properties
	ATeMotoManager();

	UFUNCTION(BlueprintCallable, Category = "TeMoto")
	void extract_nodes_umrf(FString Filename);

	UFUNCTION(BlueprintCallable, Category = "TeMoto")
	TArray<FString> GetRootActions();

	UFUNCTION(BlueprintCallable, Category = "TeMoto")
	TArray<FString> GetChildrenNode(const FString& NodeName);

	UFUNCTION(BlueprintCallable, Category = "TeMoto")
	FString GetDescription(const FString& NodeName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
