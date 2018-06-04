// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GRAI_PC.generated.h"

/**
 * 
 */
UCLASS()
class GRAI_API AGRAI_PC : public APlayerController
{
	GENERATED_BODY()
	
public:

	AGRAI_PC();

	virtual void SetupInputComponent() override;

	virtual void Possess(APawn* aPawn) override;

private:

	void LeftMouseClick();

	void SelectActor();

	void DeselectActor();

	// -----------------------------------------------------------------------------------

	/**   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FHitResult Hit;

	/** actor reference that we clicked last */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AActor* SelectedThing;

	/** pawn reference that we actually possess */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AGRAI_PlayerPawn* PossessedPawn;

public:
	
	
};
