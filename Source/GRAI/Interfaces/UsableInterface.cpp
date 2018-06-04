// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableInterface.h"


// Add default functionality here for any IUsableInterface functions that are not pure virtual.

void IUsableInterface::UseItem_Implementation(class APawn* ItemUserPawn)
{
	UE_LOG(LogTemp, Error, TEXT("IUsableInterface::UseItem_Implementation() doesn't overriden !"));
}

void IUsableInterface::AI_UseItem_Implementation(class AActor* ItemToUse)
{
	UE_LOG(LogTemp, Error, TEXT("IUsableInterface::AI_UseItem_Implementation doesn't overriden !"));
}

void IUsableInterface::Selected_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("IUsableInterface::Selected_Implementation() doesn't overriden !"));
}

void IUsableInterface::Deselected_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("IUsableInterface::Deselected_Implementation() doesn't overriden !"));
}
