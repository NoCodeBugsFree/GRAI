// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBar.generated.h"

/**
 * 
 */
UCLASS()
class GRAI_API UStatBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	/** called to set AI pawn owner */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetOwnerAIPawn(class AGRAI_AI_Charater* NewOwnerAIPawn);

	/** called to set this stat bar index  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetIndex(int32 NewIndex) { StatIndex = NewIndex; }

private:
	
	/** the index of current stat  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	int32 StatIndex = 0;
	
	/** owner AI pawn reference  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	class AGRAI_AI_Charater* OwnerAIPawn;

	// -----------------------------------------------------------------------------------

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* StatName;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* StatCurrent;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* StatMin;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* StatMax;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UProgressBar* StatDisplay;
		
};
