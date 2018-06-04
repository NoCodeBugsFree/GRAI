// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFood.h"
#include "ConstructorHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Materials/MaterialInterface.h"
#include "AI/GRAI_AI_Charater.h"
#include "Engine/DataTable.h"

// Sets default values
ABaseFood::ABaseFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** mesh  */
	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	FoodMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	FoodMesh->SetSimulatePhysics(true);
	FoodMesh->SetLinearDamping(0.8f);
	FoodMesh->SetAngularDamping(0.8f);
	SetRootComponent(FoodMesh);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		FoodMesh->SetStaticMesh(Sphere.Object);
	}

	/**  set food material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialTemplate(TEXT("/Engine/EditorMaterials/WidgetMaterial_X"));
	if (MaterialTemplate.Succeeded())
	{
		FoodMesh->SetMaterial(0, MaterialTemplate.Object);
	}

	/** set the data table asset  */
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Game/BP/Blueprints/AI/DataTables/DT"));
	if (DataTableAsset.Object)
	{
		StatsDB = DataTableAsset.Object;
	}

	/** AI Perception Stimuli Source Component  */
	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
}

void ABaseFood::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AIPerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	AIPerceptionStimuliSource->RegisterWithPerceptionSystem();
}

void ABaseFood::UseItem_Implementation(class APawn* ItemUserPawn)
{
	if ( ! IsPendingKill() )
	{
		if (AGRAI_AI_Charater* AI_Charater = Cast<AGRAI_AI_Charater>(ItemUserPawn))
		{
			if (!ensure(AI_Charater->AIStats.Num() == StatToRestore.Num()))
			{
				return;
			}

			int32 Index = 0;
			for (FAIStat& AIStat : AI_Charater->AIStats)
			{
				AIStat.CurrentStatValue += StatToRestore[Index];
				AIStat.CurrentStatValue = FMath::Clamp(AIStat.CurrentStatValue, AIStat.StatMin, AIStat.StatMax);
				Index++;
			}

			SetActorEnableCollision(false);
			SetActorHiddenInGame(true);
			Destroy();
		}
	}
}

