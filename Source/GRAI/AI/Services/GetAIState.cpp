// Fill out your copyright notice in the Description page of Project Settings.

#include "GetAIState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/GRAI_AI_Charater.h"

UGetAIState::UGetAIState()
{
	State.SelectedKeyName = "State";
	NodeName = FString("Set AI State");
	Interval = 0.25f;
	RandomDeviation = 0.f;
	bCallTickOnSearchStart = false;
}

void UGetAIState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	/** if owner is valid - set the BB State value according to AI state  */
	if (GRAI_AI_Charater)
	{
		if (OwnerComp.GetBlackboardComponent())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(State.SelectedKeyName, (uint8)GRAI_AI_Charater->CalcAndGetAIState());
		}

	} 
	else /** set the owner reference  */
	{
		if(AGRAI_AI_Charater* TestGRAI_AI_Charater = Cast<AGRAI_AI_Charater>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			GRAI_AI_Charater = TestGRAI_AI_Charater;
		}
	}
}

