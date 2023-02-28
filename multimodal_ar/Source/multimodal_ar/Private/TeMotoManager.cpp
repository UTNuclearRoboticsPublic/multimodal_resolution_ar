// Fill out your copyright notice in the Description page of Project Settings.


#include "TeMotoManager.h"


//std::unordered_multimap<std::string, std::string> graph;

// Sets default values
ATeMotoManager::ATeMotoManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeMotoManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeMotoManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeMotoManager::extract_nodes_umrf(FString graph_name)
{
	// Clean graph from previus reading
	graph.clear();
	root_actions.Empty();
	
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	TSharedPtr<FJsonObject> JsonActions;

	//FString Result = FRobofleetUnrealClientModule::Get()->RobofleetClient->GetUMRFJson(graph_name);
	FString Result = graph_name;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Result);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		if (JsonObject->HasField("umrf_actions"))
		{
			UE_LOG(LogTemp, Warning, TEXT("ok"));

			//Get Actions
			TArray<TSharedPtr<FJsonValue>> Json_umrf_actions = JsonObject->GetArrayField("umrf_actions");

			for (const auto& Element : Json_umrf_actions)
			{
				UE_LOG(LogTemp, Warning, TEXT("found action"));
				TSharedPtr<FJsonObject> action = Element.Get()->AsObject();
				FString action_name = action->GetStringField("name") + "_" + action->GetStringField("id");
				
				if (action->HasField("description"))
				{
					description[action_name] = action->GetStringField("description");
				}
				
				UE_LOG(LogTemp, Warning, TEXT("action name: %s"), *action_name);

				if (!action->HasField("parents"))
				{
					UE_LOG(LogTemp, Warning, TEXT("This node is root"));
					root_actions.Add(action_name);
				}
				if (action->HasField("execute_first"))
				{
					if (action->GetStringField("execute_first") == "true")
					{
						UE_LOG(LogTemp, Warning, TEXT("This node is root"));
						root_actions.Add(action_name);
					}					
				}
				if (action->HasField("children"))
				{
					TArray<TSharedPtr<FJsonValue>> children = action->GetArrayField("children");
					for (const auto& child : children)
					{
						FString child_node = child.Get()->AsObject()->GetStringField("name") + "_" + child.Get()->AsObject()->GetStringField("id");
						graph.insert({ {std::string(TCHAR_TO_UTF8(*action_name)), std::string(TCHAR_TO_UTF8(*child_node))} });
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("does not have umrf_actions"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail to Deserialize"));
	}
}


TArray<FString> ATeMotoManager::GetChildrenNode(const FString& NodeName)
{
	TArray<FString> children;
	auto rng = graph.equal_range(std::string(TCHAR_TO_UTF8(*NodeName)));
	//auto rng = graph.equal_range(std::string(TCHAR_TO_UTF8(*NodeName)));
	for (auto it = rng.first; it != rng.second; ++it)
	{
		children.Add(it->second.c_str());
	}
	return children;
}

TArray<FString> ATeMotoManager::GetRootActions()
{
	return root_actions;
}

FString ATeMotoManager::GetDescription(const FString& NodeName)
{
	if (description.count(NodeName) == 0) return "";
	return description[NodeName];
}