// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GRAI_AIC.generated.h"

/**
 * 
 */
UCLASS()
class GRAI_API AGRAI_AIC : public AAIController
{
	GENERATED_BODY()

	/** Behavior Tree  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	/** AI Perception Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	/** sight sense config template to our AI  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* Sight;
	
	/** hearing sense config template to our AI  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Hearing* Hearing;
	
public:

	AGRAI_AIC();

	virtual void Possess(APawn* Pawn) override;

private:

	/** calls when AI Perception Component updated */
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	/** calls when AI Perception Component updated */
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// -----------------------------------------------------------------------------------

	/** Blackboard Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;
	
	/** BehaviorTree Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	/** all seen trees   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<class ATree*> Trees;

public:
	
	FORCEINLINE class UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
};
