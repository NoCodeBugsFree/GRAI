// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/UsableInterface.h"
#include "Engine/DataTable.h"
#include "GRAI_AI_Charater.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FAIStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FAIStat() {}

	FAIStat(FName NewStatName, float NewCurrentStatValue, float NewStatMin, float NewStatMax, float NewStatDecayRate, float NewStatDesirability)
	{
		StatName = NewStatName;
		CurrentStatValue = NewCurrentStatValue;
		StatMin = NewStatMin;
		StatMax = NewStatMax;
		StatDecayRate = NewStatDecayRate;
		StatDesirability = NewStatDesirability;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
	FName StatName = FName("Enter Stat Name");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
	float CurrentStatValue = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
	float StatMin = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
	float StatMax = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
	float StatDecayRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Stats")
	float StatDesirability = 0.f;
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	/** random movement   */
	AS_Wander UMETA(DisplayName = "Wander"),

	/** try to remember trees and go there  */
	AS_Hungry UMETA(DisplayName = "Hungry"),

	/** try to find food and go there  */
	AS_LowStam UMETA(DisplayName = "LowStam")
};

UCLASS()
class GRAI_API AGRAI_AI_Charater : public ACharacter, public IUsableInterface
{
	GENERATED_BODY()

	/** removed to HUD  */
	/** stats widget   */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	// class UWidgetComponent* Stats;
	
	/* DMI reference */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DMI;

	/** Data table that represents all stats in this game  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UDataTable* StatsDB;

public:

	AGRAI_AI_Charater();

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** called to increase/decrease stamina */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ChangeStamina(float Value);

	/** called to calculate current AI state */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	EAIState CalcAndGetAIState();

	//~ Begin IUsableInterface Interface
	/** called by BT to use item by controlled AI   */
	virtual void AI_UseItem_Implementation(class AActor* ItemToUse) override;
	/** called when player select object that implements this interface  */
	virtual void Selected_Implementation() override;
	/** called when player deselect object that implements this interface  */
	virtual void Deselected_Implementation() override;
	//~ End IUsableInterface Interface
	
	/** TODO  */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	int32 GetMostDesiredStatIndex() const;

	/** Holds all stats for this type of AI  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<FAIStat> AIStats;
	
protected:

	virtual void BeginPlay() override;

	/** called to start dialog when we found a food */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Gameplay")
	void DialogCallOut(class AActor* ThingToTalkAbout);

private:

	/** called to fill AIStats array from data table */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void FillAIStats();
	
	/** called to ragdoll the character and stop all timers  */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void KillCharacter();

	/** called to handle stamina logic constantly per StaminaTickRate delay  */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StatTick(int32 StatToTick);

	// ----TODO--------------------------------------------------------------------------

	/** shows current state of this AI  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	EAIState AIState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 450.f;

	// -----------------------------------------------------------------------------------
	
	/** the initial color of the character's mesh */
	UPROPERTY()
	FLinearColor InitialColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float StatTickRate = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 MostDesiredStatIndex = 0;
	
	// -----------------------------------------------------------------------------------

public:

	FORCEINLINE float GetStamina() const { return AIStats[1].CurrentStatValue; }
	
};
