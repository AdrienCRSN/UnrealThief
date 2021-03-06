// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerEnemy.h"

#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardKeys.h"
#include "GC_UE4CPP/MainCharacter/MainCharacter.h"

AAIControllerEnemy::AAIControllerEnemy()
{
	SetupPerceptionSystem();
}

void AAIControllerEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (BTAsset != nullptr)
		RunBehaviorTree(BTAsset);

	GameModeBase = Cast<AGC_UE4CPPGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase != nullptr)
	{
		if (GameModeBase->ActorReferencer != nullptr)
		{
			if (GameModeBase->ActorReferencer->ExitDoor != nullptr)
				Blackboard->SetValueAsVector(BBKeys::ExitDoorLocation, GameModeBase->ActorReferencer->ExitDoor->GetActorLocation());
		}
	}

	Blackboard->SetValueAsInt(BBKeys::FoodSpotIndex, 0); // default value
}

void AAIControllerEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GameModeBase != nullptr)
	{
		if (GameModeBase->bVictory || GameModeBase->bDefeat)
			Blackboard->SetValueAsBool(BBKeys::StopLogic, true);
	}
}

void AAIControllerEnemy::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (Cast<AMainCharacter>(Actor))
	{
		Blackboard->SetValueAsBool(BBKeys::CanSeePlayer, Stimulus.WasSuccessfullySensed());
	}
}

void AAIControllerEnemy::SetupPerceptionSystem()
{
	// Create and set the perception component
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SetPerceptionComponent(*AIPerception);

	// Create and initialize the sight config object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
	SightConfig->PeripheralVisionAngleDegrees = 135.0f;
	SightConfig->SetMaxAge(3.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Add sight config to perception component
	GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerEnemy::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
