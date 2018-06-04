// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GRAI_PlayerPawn.generated.h"

UCLASS()
class GRAI_API AGRAI_PlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	
	AGRAI_PlayerPawn();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	virtual void BeginPlay() override;

private:

	void CreateHUD();

	/** widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStatBlock> HUDWidgetClass;

	/** current active widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UStatBlock* HUD;

public:	

	FORCEINLINE class UStatBlock* GetHUD() const { return HUD; }
};
