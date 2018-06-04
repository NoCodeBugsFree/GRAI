// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckStats.generated.h"

/**
 * 
 */
UCLASS()
class GRAI_API UCheckStats : public UBTService
{
	GENERATED_BODY()

public:

	UCheckStats();

private:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	/** flag that shows whether we should start find the food or not  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector FindFood; 
	
	/** stamina value at which we start searching for food  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float StaminaLowLevel = 25.f;
};
