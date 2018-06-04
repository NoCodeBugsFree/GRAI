// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_UseItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GRAI/AI/GRAI_AI_Charater.h"
#include "DrawDebugHelpers.h"

UAI_UseItem::UAI_UseItem()
{
	DesiredObject.SelectedKeyName = "DesiredObject";
	NodeName = FString("AI Use Item");
}

EBTNodeResult::Type UAI_UseItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
				/** if we have valid actor in blackboard  */
				if (AActor* DesiredActor = Cast<AActor>(BB->GetValueAsObject(DesiredObject.SelectedKeyName)))
				{
					/** if object stored in BB is valid and implements UsableInterface  */
					if (DesiredActor->GetClass()->ImplementsInterface(UUsableInterface::StaticClass()))
					{
						/** check whether the AI pawn and desired actor is in UseRange  */
						FVector AI_PawnEyesLocation; FRotator AI_PawnEyesRotation;
						AIPawn->GetActorEyesViewPoint(AI_PawnEyesLocation, AI_PawnEyesRotation);
						FVector DesiredActorLocation = DesiredActor->GetActorLocation();
						bool bInUseRange = (AI_PawnEyesLocation - DesiredActorLocation).Size() <= UseRange;

						/** check whether the AI pawn can see the desired actor  */
						bool bCanSee = false;

						FVector Start = AI_PawnEyesLocation;
						FVector End = DesiredActorLocation;
						
						FHitResult Hit;
						FCollisionQueryParams CollisionQueryParams;
						CollisionQueryParams.AddIgnoredActor(AIPawn);
						
						if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionQueryParams))
						{
							if (Hit.GetActor() == DesiredActor)
							{
								bCanSee = true;
							}
						}
						
						// DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 10.f, (uint8)0, 10.f);

						if (bCanSee && bInUseRange)
						{
							IUsableInterface::Execute_AI_UseItem(AIPawn, DesiredActor);
						}
					}
					
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
