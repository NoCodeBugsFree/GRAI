// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Tree.generated.h"

/**
 * 
 */
UCLASS()
class GRAI_API ATree : public AStaticMeshActor
{
	GENERATED_BODY()

	/* scene component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ROOT;
	
	/** trunk mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TrunkMesh;

	/** AI Perception Stimuli Source Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;
	
public:

	ATree();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

private:

	/** called to spawn a food item */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnFood();

	// -----------------------------------------------------------------------------------

	/** shows whether this tree is spawning food or not  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bShouldSpawn : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 PrimaryStatIndex = 2;

	/** spawn food delay  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float SpawnFoodDelay = 1.f;

	/** food life span coefficient - approximate amount of food items around the tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float FoodLifeSpan = 3.f;

	/** spawn radius  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float SpawnRadius = 250.f;
	
	/** food class to spawn  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseFood> FoodClass;
	
};
