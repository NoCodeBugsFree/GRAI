// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GetAIState.generated.h"

/**
 * 
 */
UCLASS()
class GRAI_API UGetAIState : public UBTService
{
	GENERATED_BODY()

private:

	UGetAIState();

	/** update next tick interval
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** state of the AI  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector State; 

	/** AI owner reference  */
	UPROPERTY()
	class AGRAI_AI_Charater* GRAI_AI_Charater;

};
