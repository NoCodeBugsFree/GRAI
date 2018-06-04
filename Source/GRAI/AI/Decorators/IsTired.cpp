// Fill out your copyright notice in the Description page of Project Settings.

#include "IsTired.h"
#include "BehaviorTree/BlackboardComponent.h"

UIsTired::UIsTired()
{
	bTired.SelectedKeyName = "bTired";
	NodeName = FString("Is AI Tired ?");
	FlowAbortMode = EBTFlowAbortMode::Self;
	SetIsInversed(false);
}

bool UIsTired::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		return BB->GetValueAsBool(bTired.SelectedKeyName);
	}
	return false;
}
