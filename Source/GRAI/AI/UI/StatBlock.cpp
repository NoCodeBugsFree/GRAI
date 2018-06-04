// Fill out your copyright notice in the Description page of Project Settings.

#include "StatBlock.h"
#include "Blueprint/UserWidget.h"
#include "ConstructorHelpers.h"
#include "Components/VerticalBox.h"
#include "AI/GRAI_AI_Charater.h"
#include "AI/UI/StatBar.h"
#include "Engine/DataTable.h"

UStatBlock::UStatBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/**  set the widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> StatBarWidgetClassTemplate(TEXT("/Game/BP/Blueprints/AI/UI/WBP_StatBar"));
	if (StatBarWidgetClassTemplate.Class)
	{
		StatBarWidgetClass = StatBarWidgetClassTemplate.Class;
	}

	/** set the data table asset  */
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Game/BP/Blueprints/AI/DataTables/DT"));
	if (DataTableAsset.Object)
	{
		StatsDB = DataTableAsset.Object;
	}
}

void UStatBlock::NativeConstruct()
{
	Super::NativeConstruct();
	
	/** read all data table data to array  */
	if (StatsDB)
	{
		FString ContextString;
		TArray<FName> RowNames;
		RowNames = StatsDB->GetRowNames();
		
		int32 Index = 0;
		for (FName& RowName : RowNames)
		{
			CreateStatBarWidget(Index);
			Index++;
		}

		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StatsDB == NULL!"));
	}
}

void UStatBlock::UpdateHUD()
{
	for (UStatBar* StatBar : StatBars)
	{
		if (StatBar)
		{
			StatBar->SetOwnerAIPawn(OwnerAIPawn);
		}
	}
}

void UStatBlock::ClearPawn()
{
	for (UStatBar* StatBar : StatBars)
	{
		if (StatBar)
		{
			StatBar->SetOwnerAIPawn(nullptr);
		}
	}
}

void UStatBlock::CreateStatBarWidget(int32 StatIndex)
{
	if (StatBarWidgetClass)
	{
		UStatBar* CurrentStatBar = CreateWidget<UStatBar>(GetWorld(), StatBarWidgetClass);
		if (CurrentStatBar)
		{
			StatsHolder->AddChildToVerticalBox(CurrentStatBar);
			CurrentStatBar->SetIndex(StatIndex);
			StatBars.AddUnique(CurrentStatBar);
		}
	}
}
