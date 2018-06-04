// Fill out your copyright notice in the Description page of Project Settings.

#include "FindRandomLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Navigation/NavigationSystem.h"

UFindRandomLocation::UFindRandomLocation()
{
	MoveToLocation.SelectedKeyName = "MoveToLocation";
	NodeName = FString("Find Random Location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		if (AIPawn)
		{
			UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
			if (BB)
			{
				FNavLocation RandomPoint;
				if (GetWorld()->GetNavigationSystem()->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), SearchRadius, RandomPoint))
				{
					BB->SetValueAsVector(MoveToLocation.SelectedKeyName, RandomPoint.Location);
					
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
