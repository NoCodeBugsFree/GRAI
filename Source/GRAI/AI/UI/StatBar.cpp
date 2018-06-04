// Fill out your copyright notice in the Description page of Project Settings.

#include "StatBar.h"
#include "AI/GRAI_AI_Charater.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UStatBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnerAIPawn)
	{
		///** stat name  */
		FText StatNameText = FText::FromString(OwnerAIPawn->AIStats[StatIndex].StatName.ToString());
		StatName->SetText(StatNameText);
		
		/** stat current  */
		FString StatCurrentString = FString::Printf(TEXT("%.1f"), OwnerAIPawn->AIStats[StatIndex].CurrentStatValue);
		FText StatCurrentText = FText::FromString(StatCurrentString);
		StatCurrent->SetText(StatCurrentText);

		/** stat min  */
		FString StatMinString = FString::Printf(TEXT("%.1f"), OwnerAIPawn->AIStats[StatIndex].StatMin);
		FText StatMinText = FText::FromString(StatMinString);
		StatMin->SetText(StatMinText);

		/** stat max  */
		FString StatMaxString = FString::Printf(TEXT("%.1f"), OwnerAIPawn->AIStats[StatIndex].StatMax);
		FText StatMaxText = FText::FromString(StatMaxString);
		StatMax->SetText(StatMaxText);
		
		/** percentage  */
		float CurrentStatValue = OwnerAIPawn->AIStats[StatIndex].CurrentStatValue;
		float StatMax = OwnerAIPawn->AIStats[StatIndex].StatMax;
		float StatMin = OwnerAIPawn->AIStats[StatIndex].StatMin;
		float Percentage = (CurrentStatValue - StatMin) / (StatMax - StatMin);
		StatDisplay->SetPercent(Percentage);
	}
}

void UStatBar::SetOwnerAIPawn(class AGRAI_AI_Charater* NewOwnerAIPawn)
{
	OwnerAIPawn = NewOwnerAIPawn;
}
