// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataProviders/AIDataProvider_QueryParams.h"
#include "DataProvider.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Owner\'s properties"))
class UDataProvider : public UAIDataProvider_QueryParams
{
	GENERATED_BODY()

public:

	virtual void BindData(const UObject& Owner, int32 RequestId) override;
	
protected:
	
	UPROPERTY()
	int32 RequiredStatIndex;
	
};
