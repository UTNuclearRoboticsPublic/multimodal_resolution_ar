#include "HttpActor.h"
#include "message_structs.h"
#include "RobofleetUnrealClientModule.h"
#include "RobofleetClientBase.h"
#include "Misc/DateTime.h"

// Sets default values
AHttpActor::AHttpActor()
{
	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();
}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	//TODO: Read Host URL from config file
	//HostURL = "192.168.1.11:3000";
	Super::BeginPlay();
}

/*Http call*/
// TODO: Callback might be ignored if another request is made
void AHttpActor::RequestAnchorID(FString RepresentationID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);
	//This is the url on which to process the request

	FString FullURL = "http://" + HostURL + "/" + AugreDBTable + "?RepresentationId=eq." + RepresentationID;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FullURL)

		Request->SetURL("http://" + HostURL + "/" + AugreDBTable + "?RepresentationId=eq." + RepresentationID);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();

	//UE_LOG(LogTemp,log,TEXT("Anchor ID request sent to DB"))
}

/*Assigned function on successfull http call*/
void AHttpActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//UE_LOG(LogTemp, log, TEXT("AnchorID Request was succesfull: " + bWasSuccessful ? "true" : "false"));
	if (bWasSuccessful)
	{
		//Create a pointer to hold the json serialized data
		TSharedPtr<FJsonObject> JsonObject;

		//Create a reader pointer to read the json data
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		//Deserialize the json data given Reader and the actual object to deserialize

		// Array of json objects at top level of json
		TArray<TSharedPtr<FJsonValue>> JsonArray;

		// Deserialize the json data given Reader and the actual object to deserialize
		if (FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			if (JsonArray.IsValidIndex(0))
			{
				//Get the value of the json object by field name
				FString AnchorID = JsonArray[0]->AsObject()->GetStringField("AsaId");
				FString RepID = JsonArray[0]->AsObject()->GetStringField("RepresentationId");
				OnNewAnchorIDReceived.Broadcast(RepID, AnchorID);
				FRobofleetUnrealClientModule::Get()->RobofleetClient->OnNewAnchorSeen.Broadcast(AnchorID);
			}
			else
			{
				FString Prefix = "http://" + HostURL + "/" + AugreDBTable + "?RepresentationId = eq.";
				FString RequestURL = Request->GetURL();
				FString RepIDRequested = RequestURL.RightChop(Prefix.Len());
				OnAnchorIDNotFound.Broadcast(RepIDRequested);
			}
		}
	}
}

void AHttpActor::SubmitDatabaseEntry(FString RepresentationID, FString AnchorID, FDateTime CurTimeStamp)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("AsaId"), *FString::Printf(TEXT("%s"), *AnchorID));
	JsonObject->SetStringField(TEXT("RepresentationId"), *FString::Printf(TEXT("%s"), *RepresentationID));
	JsonObject->SetNumberField(TEXT("TimeStamp"), (double)CurTimeStamp.ToUnixTimestamp());

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	UE_LOG(LogTemp, Warning, TEXT("SerializedJson: %s"), *OutputString)

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);
	//This is the url on which to process the request
	//Request->SetURL("http://" + HostURL + "/" + AugreDBTable + "?on_conflict=RepresentationId");
	Request->SetURL("http://" + HostURL + "/" + AugreDBTable + "?");
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetContentAsString(OutputString);

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnSubmitDatabaseEntryCompleted);
	Request->ProcessRequest();
}


void AHttpActor::OnSubmitDatabaseEntryCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Database entry submission failed"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully Saved Anchor to Database"))
	}
}


void AHttpActor::DeleteDatabaseEntry(FString RepresentationID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);
	//This is the url on which to process the request

	FString FullURL = "http://" + HostURL + "/" + AugreDBTable + "?RepresentationId=eq." + RepresentationID;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FullURL)

		Request->SetURL("http://" + HostURL + "/" + AugreDBTable + "?RepresentationId=eq." + RepresentationID);
	Request->SetVerb("DELETE");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();

}

void AHttpActor::OnDeleteDatabaseEntryCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Database delete submission failed"))
	}
}

void AHttpActor::SetWorldGeoOriginOnRobofleet()
{
	NavSatFix WorldOrigin; // TODO: This will be requested from the gemstone.
	// NRG Lab Coordinates from Google Maps: Lat: 30.28820584690383, Lon: -97.73759835043117
	WorldOrigin.latitude = 30.28820584690383;
	WorldOrigin.longitude = -97.73759835043117;
	WorldOrigin.altitude = 10;

	if (FRobofleetUnrealClientModule::Get()->IsSessionRunning())
	{
		FRobofleetUnrealClientModule::Get()->RobofleetClient->SetWorldGeoOrigin(WorldOrigin);
	}

}

void AHttpActor::ScanDatabaseForAsaID()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnQueryDatabaseReceived);  //check this
	//This is the url on which to process the request

	FString FullURL = "http://" + HostURL + "/" + AugreDBTable;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FullURL)

		Request->SetURL("http://" + HostURL + "/" + AugreDBTable);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();

	//UE_LOG(LogTemp,log,TEXT("Anchor ID request sent to DB"))
}


void AHttpActor::OnQueryDatabaseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT(" ===== OnQueryDatabaseReceived ===== "))
		//UE_LOG(LogTemp, log, TEXT("AnchorID Request was succesfull: " + bWasSuccessful ? "true" : "false"));
		if (bWasSuccessful)
		{
			//Create a pointer to hold the json serialized data
			TSharedPtr<FJsonObject> JsonObject;

			//Create a reader pointer to read the json data
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			//Deserialize the json data given Reader and the actual object to deserialize

			// Array of json objects at top level of json
			TArray<TSharedPtr<FJsonValue>> JsonArray;

			// Deserialize the json data given Reader and the actual object to deserialize
			if (FJsonSerializer::Deserialize(Reader, JsonArray))
			{
				UE_LOG(LogTemp, Warning, TEXT("Anchor ID request sent to DB %d"), JsonArray.Num());
				for (int i = 0; i < JsonArray.Num(); i++)
				{
					if (JsonArray.IsValidIndex(i))
					{
						//Get the value of the json object by field name
						FString AnchorID = JsonArray[i]->AsObject()->GetStringField("AsaId");
						UE_LOG(LogTemp, Warning, TEXT("Found Anchor ID: %s"), *AnchorID);

						//FString RepID = JsonArray[0]->AsObject()->GetStringField("RepresentationId");
						//OnNewAnchorIDReceived.Broadcast(RepID, AnchorID);
						FRobofleetUnrealClientModule::Get()->RobofleetClient->OnNewAnchorSeen.Broadcast(AnchorID);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Anchor ID invalid "));
						/*FString Prefix = "http://" + HostURL + "/" + AugreDBTable + "?RepresentationId = eq.";
						FString RequestURL = Request->GetURL();
						FString RepIDRequested = RequestURL.RightChop(Prefix.Len());
						OnAnchorIDNotFound.Broadcast(RepIDRequested);*/
					}
				}
			}
		}
}