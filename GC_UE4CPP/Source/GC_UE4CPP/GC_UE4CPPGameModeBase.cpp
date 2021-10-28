// Copyright Epic Games, Inc. All Rights Reserved.


#include "GC_UE4CPPGameModeBase.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGC_UE4CPPGameModeBase::AGC_UE4CPPGameModeBase()
{
	
}

void AGC_UE4CPPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActorReferencer::StaticClass(), OutActors);
	ActorReferencer = Cast<AActorReferencer>(OutActors[0]);

	HUDBase = Cast<AInterfaceCreation>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUDBase->InitWidgets();
	
	Play();
	InitAI();
}

void AGC_UE4CPPGameModeBase::InitAI()
{
	// 2 initial AI
	SpawnAI();
	// SpawnAIWithDelay(1);

	// one more after a minute
	// SpawnAIWithDelay(60.0f);
}

void AGC_UE4CPPGameModeBase::SpawnAI()
{
	ActiveAI = ActiveAI + 1;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	FRotator Rotator;
	AAIEnemyCharacter* aiEnemyCharacter = GetWorld()->SpawnActor<AAIEnemyCharacter>(EnemyCharacter, ActorReferencer->EditDoor->GetActorLocation(), Rotator, SpawnParameters);

	if (FoodNb < 5)
	{
		aiEnemyCharacter->CreateAndAttachFood();
		FoodNb = FoodNb + 1;
	}
}

void AGC_UE4CPPGameModeBase::SpawnAIWithDelay(float Delay)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGC_UE4CPPGameModeBase::SpawnAI, Delay, false);
}

void AGC_UE4CPPGameModeBase::RemoveAI()
{
	ActiveAI = ActiveAI - 1;
	if (ActiveAI <= 0)
	{
		SpawnAI();
	} else
	{
		SpawnAIWithDelay(UKismetMathLibrary::RandomFloatInRange(0.0f, 5.0f));
	}
}

void AGC_UE4CPPGameModeBase::Victory()
{
	bVictory = true;
	bDefeat = false;
	bPlaying = false;

	HUDBase->ShowEndHUD(true);
}

void AGC_UE4CPPGameModeBase::Defeat()
{
	bVictory = false;
	bDefeat = true;
	bPlaying = false;

	HUDBase->ShowEndHUD(false);
}

void AGC_UE4CPPGameModeBase::Play()
{
	bVictory = false;
	bDefeat = false;
	bPlaying = true;

	HUDBase->ShowPlayHUD();
}

void AGC_UE4CPPGameModeBase::AddFood()
{
	FoodCurrentQuantity = UKismetMathLibrary::Min(FoodCurrentQuantity + 1, FoodMaxQuantity);
	if (FoodCurrentQuantity == 5 )
	{
		Victory();
	}
}
