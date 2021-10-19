// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodProgressBar.h"
#include "GC_UE4CPP/GC_UE4CPPGameModeBase.h"

#include "Components/ProgressBar.h"

void UFoodProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	GameModeBase = Cast<AGC_UE4CPPGameModeBase>(GetWorld()->GetAuthGameMode());
	WidgetFoodProgressBar->SetPercent(0);
}

void UFoodProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	WidgetFoodProgressBar->SetPercent(GameModeBase->FoodCurrentQuantity/GameModeBase->FoodMaxQuantity);
}
