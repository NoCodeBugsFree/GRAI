// Fill out your copyright notice in the Description page of Project Settings.

#include "DataProvider.h"
#include "AI/GRAI_AI_Charater.h"

void UDataProvider::BindData(const UObject& Owner, int32 RequestId)
{
	const AGRAI_AI_Charater* Character = Cast<const AGRAI_AI_Charater>(&Owner);
	if (Character)
	{
		RequiredStatIndex = Character->GetMostDesiredStatIndex();
	}
}