// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindNextFoodSpot.h"

#include "AIControllerEnemy.h"
#include "BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetArrayLibrary.h"

UBTTask_FindNextFoodSpot::UBTTask_FindNextFoodSpot(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Find Next Food Spot");
}

EBTNodeResult::Type UBTTask_FindNextFoodSpot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerEnemy* Controller = Cast<AAIControllerEnemy>(OwnerComp.GetAIOwner());

	AGC_UE4CPPGameModeBase* GameModeBase = Cast<AGC_UE4CPPGameModeBase>(GetWorld()->GetAuthGameMode());
	TArray<AActor*> FoodSpots = GameModeBase->ActorReferencer->FoodSpots;
	int Length = FoodSpots.Num();
	int CurrentIndex = Controller->GetBlackboardComponent()->GetValueAsInt(BBKeys::FoodSpotIndex);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::FromInt(CurrentIndex));

	int NextIndex = CurrentIndex+1;
	if (NextIndex == Length)
	{
		NextIndex = 0;
	}

	// Set the BB values
	Controller->GetBlackboardComponent()->SetValueAsObject(BBKeys::FoodSpot, FoodSpots[CurrentIndex]);
	Controller->GetBlackboardComponent()->SetValueAsInt(BBKeys::FoodSpotIndex, NextIndex); // go to next spot
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
