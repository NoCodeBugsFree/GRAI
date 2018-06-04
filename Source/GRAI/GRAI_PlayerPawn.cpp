// Fill out your copyright notice in the Description page of Project Settings.

#include "GRAI_PlayerPawn.h"
#include "Blueprint/UserWidget.h"
#include "ConstructorHelpers.h"
#include "AI/UI/StatBlock.h"

// Sets default values
AGRAI_PlayerPawn::AGRAI_PlayerPawn()
{
	/**  set the widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClassBPClass(TEXT("/Game/BP/Blueprints/AI/UI/WBP_StatBlock"));
	if (HUDWidgetClassBPClass.Class)
	{
		HUDWidgetClass = HUDWidgetClassBPClass.Class;
	}
}

// Called when the game starts or when spawned
void AGRAI_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	CreateHUD();
}

void AGRAI_PlayerPawn::CreateHUD()
{
	if (HUDWidgetClass)
	{
		HUD = CreateWidget<UStatBlock>(GetWorld(), HUDWidgetClass);
		if (HUD)
		{
			HUD->AddToViewport();
		}
	}
}

// Called to bind functionality to input
void AGRAI_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

