// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "StatTest.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Amount of needed stat"))
class GRAI_API UStatTest : public UEnvQueryTest
{
	GENERATED_BODY()

public:

	UStatTest();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	
protected:

	/** what stat we need ?  */
	UPROPERTY(EditDefaultsOnly, Category = "Test")
	FAIDataProviderIntValue NeededStatIndex;

};
