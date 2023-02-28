#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "HttpActor.generated.h"

//OnNewAnchorReceived event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewAnchorIDReceived, FString, RepresentationID, FString, AnchorID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnchorIDNotFound, FString, RepresentationID);

UCLASS(Blueprintable)
class AHttpActor : public AActor
{
	GENERATED_BODY()

	FString HostURL = "192.168.11.11:3000";
	FString AugreDBTable = "augredbtable";

public:
	FHttpModule* Http;

	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable)
		void RequestAnchorID(FString RepresentationID);

	UFUNCTION(BlueprintCallable)
		void ScanDatabaseForAsaID();

	// Get all asa_id on DB
	void OnQueryDatabaseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/*Assign this function to call when the GET request processes successfully*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void SubmitDatabaseEntry(FString RepresentationID, FString AnchorID, FDateTime CurTimeStamp);

	void OnSubmitDatabaseEntryCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void DeleteDatabaseEntry(FString RepresentationID);

	void OnDeleteDatabaseEntryCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
		void SetWorldGeoOriginOnRobofleet();

	// Sets default values for this actor's properties
	AHttpActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "ASA ID Database")
		FOnNewAnchorIDReceived OnNewAnchorIDReceived;

	UPROPERTY(BlueprintAssignable, Category = "ASA ID Database")
		FOnAnchorIDNotFound OnAnchorIDNotFound;



};
