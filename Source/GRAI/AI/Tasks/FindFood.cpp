// Fill out your copyright notice in the Description page of Project Settings.

#include "FindFood.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pickups/BaseFood.h"

UFindFood::UFindFood()
{
	DesiredObject.SelectedKeyName = "DesiredObject";
	NodeName = FString("Find Food CPP");
}

EBTNodeResult::Type UFindFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
				/** try to find good food  */
				TArray<AActor*> GoodFoods;
				UGameplayStatics::GetAllActorsWithTag(this, FName("GoodFood"), GoodFoods);

				AActor* BestTarget = nullptr;
				float Distance = MAX_FLT;
				FVector AIPawnLocation = AIPawn->GetActorLocation();

				if (GoodFoods.Num() > 0)
				{
					for (AActor* TestFood : GoodFoods)
					{
						if (TestFood)
						{
							FVector TestFoodLocation = TestFood->GetActorLocation();
							float CurrentDistance = (AIPawnLocation - TestFoodLocation).Size();
							if (CurrentDistance <= Distance)
							{
								Distance = CurrentDistance;
								BestTarget = TestFood;
							}
						}
					}

					if (BestTarget)
					{
						BB->SetValueAsObject(DesiredObject.SelectedKeyName, BestTarget);
						return EBTNodeResult::Succeeded;
					}
				}

				/** try to find bad food  */
				TArray<AActor*> BadFoods;
				UGameplayStatics::GetAllActorsWithTag(this, FName("BadFood"), BadFoods);
				if(BadFoods.Num() > 0)
				{
					for (AActor* TestFood : BadFoods)
					{
						if (TestFood)
						{
							FVector TestFoodLocation = TestFood->GetActorLocation();
							float CurrentDistance = (AIPawnLocation - TestFoodLocation).Size();
							if (CurrentDistance <= Distance)
							{
								Distance = CurrentDistance;
								BestTarget = TestFood;
							}
						}
					}

					if (BestTarget)
					{
						BB->SetValueAsObject(DesiredObject.SelectedKeyName, BestTarget);
						return EBTNodeResult::Succeeded;
					}
				}	
			}
		}
	}
	return EBTNodeResult::Failed;
}
