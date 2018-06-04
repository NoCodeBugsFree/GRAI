// Fill out your copyright notice in the Description page of Project Settings.

#include "GRAI_PC.h"
#include "GRAI_PlayerPawn.h"
#include "AI/GRAI_AI_Charater.h"
#include "AI/UI/StatBlock.h"

AGRAI_PC::AGRAI_PC()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
}

void AGRAI_PC::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AGRAI_PC::LeftMouseClick);
}

void AGRAI_PC::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	if(AGRAI_PlayerPawn* TestGRAI_PlayerPawn = Cast<AGRAI_PlayerPawn>(aPawn))
	{
		PossessedPawn = TestGRAI_PlayerPawn;
	}
}

void AGRAI_PC::LeftMouseClick()
{
	if ( GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, Hit))
	{
		if (Hit.GetActor())
		{
			if (Hit.GetActor()->GetClass()->ImplementsInterface(UUsableInterface::StaticClass()))
			{
				/** if we have a selected one, deselect it  */
				if (SelectedThing)
				{
					DeselectActor();
				}
				else /** if not - select it  */
				{
					SelectActor();
				}
			} 
			else /** if we hit anything else - deselect previous selected  */
			{
				DeselectActor();
			}
		}
	}
}

void AGRAI_PC::SelectActor()
{
	SelectedThing = Hit.GetActor();
	IUsableInterface::Execute_Selected(SelectedThing);

	if (AGRAI_AI_Charater* GRAI_AI_Charater = Cast<AGRAI_AI_Charater>(SelectedThing))
	{
		if (PossessedPawn && PossessedPawn->GetHUD())
		{
			PossessedPawn->GetHUD()->SetOwnerAIPawn(GRAI_AI_Charater);
			PossessedPawn->GetHUD()->UpdateHUD();
			PossessedPawn->GetHUD()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AGRAI_PC::DeselectActor()
{
	if (SelectedThing)
	{
		IUsableInterface::Execute_Deselected(SelectedThing);
	}

	SelectedThing = nullptr;

	if (PossessedPawn && PossessedPawn->GetHUD())
	{
		PossessedPawn->GetHUD()->ClearPawn();
		PossessedPawn->GetHUD()->SetVisibility(ESlateVisibility::Hidden);
	}
}

