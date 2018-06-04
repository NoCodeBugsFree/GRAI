// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckStats.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GRAI/AI/GRAI_AI_Charater.h"

UCheckStats::UCheckStats()
{
	FindFood.SelectedKeyName = "bTired";
	NodeName = FString("Stat Check");
	Interval = 0.25f;
	RandomDeviation = 0.f;
	bCallTickOnSearchStart = true;
}

void UCheckStats::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		if (AGRAI_AI_Charater* AICharacter = Cast<AGRAI_AI_Charater>(AIController->GetPawn()))
		{
			if (OwnerComp.GetBlackboardComponent())
			{
				bool bTired = AICharacter->GetStamina() <= StaminaLowLevel;
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(FindFood.SelectedKeyName, bTired);
			}
		}
	}
}
