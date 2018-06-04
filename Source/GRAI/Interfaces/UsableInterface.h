// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UUsableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRAI_API IUsableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/** called by every item when pawn send a message to it */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void UseItem(class APawn* ItemUserPawn); /** TODO use FAIStat as seconds param  */
	virtual void UseItem_Implementation(class APawn* ItemUserPawn);

	/** called by BT to use item by controlled AI   */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void AI_UseItem(class AActor* ItemToUse);
	virtual void AI_UseItem_Implementation(class AActor* ItemToUse);

	/** called when player select object that implements this interface  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void Selected();
	virtual void Selected_Implementation();

	/** called when player deselect object that implements this interface  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void Deselected();
	virtual void Deselected_Implementation();
	
};
