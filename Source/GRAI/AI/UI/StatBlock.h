// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBlock.generated.h"

/**
 * 
 */
UCLASS()
class GRAI_API UStatBlock : public UUserWidget
{
	GENERATED_BODY()

	/** Data table that represents all stats in this game  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UDataTable* StatsDB;
	
public:

	UStatBlock(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	/** called to set AI pawn owner */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetOwnerAIPawn(class AGRAI_AI_Charater* NewOwnerAIPawn) { OwnerAIPawn = NewOwnerAIPawn; }

	/** called to set valid AI pawn to all StatBars */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHUD();

	/** called to clear AI pawn to all StatBars */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ClearPawn();

private:
	
	/** called create and add one stat bar to this widget */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void CreateStatBarWidget(int32 StatIndex);
	
	/** stat bar that contains this widget  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<class UStatBar*> StatBars;

	/** owner AI pawn reference  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	class AGRAI_AI_Charater* OwnerAIPawn;

	/** vertical box that holding stat bars  */
	UPROPERTY(meta = (BindWidget), Category = "Config", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UVerticalBox* StatsHolder;

	/** widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> StatBarWidgetClass;
	
};
