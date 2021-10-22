// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightAnimation.h"

UKnightAnimation::UKnightAnimation()
{
}

inline void UKnightAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	GameModeBase = Cast<AGC_UE4CPPGameModeBase>(GetWorld()->GetAuthGameMode());
}

void UKnightAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AActor* OwningActor = GetOwningActor();
	if (IsValid(OwningActor))
	{
		Speed = OwningActor->GetVelocity().Size();
	}

	if (IsValid(GameModeBase))
	{
		bVictory = GameModeBase->bVictory;
		bDefeat = GameModeBase->bDefeat;
		bPlaying = GameModeBase->bPlaying;
	}
}