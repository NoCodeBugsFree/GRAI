// Fill out your copyright notice in the Description page of Project Settings.

#include "StatTest.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Pickups/BaseFood.h"

UStatTest::UStatTest()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_Actor::StaticClass();
	FloatValueMin.DefaultValue = 0.f;
	FloatValueMax.DefaultValue = 1.f;
}

void UStatTest::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();

	/** owner is dead  */
	if (QueryOwner == nullptr)
	{
		return;
	}

	/** the min stat value to look for  */
	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	const float MinThresholdValue = FloatValueMin.GetValue();

	/** the max stat value to look for  */
	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	const float MaxThresholdValue = FloatValueMax.GetValue();

	/** the stat index we are actually looking for  */
	NeededStatIndex.BindData(QueryOwner, QueryInstance.QueryID);
	const int32 NeededStat = NeededStatIndex.GetValue();

	/** loop through all items */
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		/** TODO null here  */
		const ABaseFood* BaseFood = Cast<ABaseFood>(ItemActor);
		const float ItemScore = 0.f;
		//const float ItemScore = BaseFood ? BaseFood->GetStatRestore(NeededStat) : 0.f;
		It.SetScore(TestPurpose, FilterType, ItemScore, MinThresholdValue, MaxThresholdValue);
	}
}

FText UStatTest::GetDescriptionTitle() const
{
	return FText::FromString(TEXT("This is a title version: Amount of needed stat"));
}
