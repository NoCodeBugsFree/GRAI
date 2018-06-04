// Fill out your copyright notice in the Description page of Project Settings.

#include "FindDesiredObject.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GRAI/Pickups/BaseFood.h"
#include "GRAI/Pickups/Tree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

UFindDesiredObject::UFindDesiredObject()
{
	DesiredObject.SelectedKeyName = "DesiredObject";
	NodeName = FString("Find Desired Object");
	
	WhatToFind = ABaseFood::StaticClass();
	bClosestToAI = true;
}

EBTNodeResult::Type UFindDesiredObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		APawn* AIPawn = AIController->GetPawn();
		if (AIPawn)
		{
			UAIPerceptionComponent* AIPerceptionComponent = AIController->GetPerceptionComponent();
			if (AIPerceptionComponent)
			{
				UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
				if (BB)
				{
					/** get all perceived actors  */
					TArray<AActor*> PerceivedActors;
					AIPerceptionComponent->GetKnownPerceivedActors(UAISenseConfig_Sight::StaticClass(), PerceivedActors);

					/** try to find closest desired actor  */
					AActor* BestActor = nullptr;
					float Distance = MAX_FLT;

					for (AActor* TestActor : PerceivedActors)
					{
						if (TestActor->GetClass()->IsChildOf(WhatToFind.GetDefaultObject()->GetClass()))
						{
							//UE_LOG(LogTemp, Error, TEXT("You found: %s"), *TestActor->GetName());
							FVector AIPawnLocation = AIPawn->GetActorLocation();
							FVector TestActorLocation = TestActor->GetActorLocation();
							float CurrentDistance = (AIPawnLocation - TestActorLocation).Size();

							/** find closest desired actor  */
							if (CurrentDistance < Distance)
							{
								BestActor = TestActor;
								Distance = CurrentDistance;
							}
						}
					}

					/** if we don't need closest actor - pick random if we have enough  */
					if (bClosestToAI == false)
					{
						if (PerceivedActors.Num() < 2)
						{
							BB->SetValueAsObject(DesiredObject.SelectedKeyName, nullptr);
							return EBTNodeResult::Failed;
						}
						else
						{
							int32 RandomIndex = FMath::RandRange(0, PerceivedActors.Num() - 1);
							if (PerceivedActors.IsValidIndex(RandomIndex))
							{
								BB->SetValueAsObject(DesiredObject.SelectedKeyName, PerceivedActors[RandomIndex]);
								return EBTNodeResult::Succeeded;
							}
						}
					}

					BB->SetValueAsObject(DesiredObject.SelectedKeyName, BestActor);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
