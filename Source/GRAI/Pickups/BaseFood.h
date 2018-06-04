// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UsableInterface.h"
#include "BaseFood.generated.h"

UCLASS()
class GRAI_API ABaseFood : public AActor, public IUsableInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* FoodMesh;

	/** AI Perception Stimuli Source Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

	/** Data table that represents all stats in this game  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UDataTable* StatsDB;
	
public:	

	ABaseFood();
	
	virtual void PostInitializeComponents() override;

	//~ Begin IUsableInterface
	virtual void UseItem_Implementation(class APawn* ItemUserPawn) override;
	//~ End IUsableInterface

private:

	/** stats to restore by this item  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<float> StatToRestore = { 10.f, 10.f, 50.f, 0.f, 0.f, 0.f, 0.f };
	
public:

	FORCEINLINE class UStaticMeshComponent* GetFoodMesh() const { return FoodMesh; }
};
