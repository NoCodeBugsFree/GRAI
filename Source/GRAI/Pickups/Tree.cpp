// Fill out your copyright notice in the Description page of Project Settings.

#include "Tree.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "BaseFood.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ATree::ATree()
{
	/* scene component  */
	ROOT = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(ROOT);

	/** trunk mesh component  */
	TrunkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrunkMesh"));
	TrunkMesh->SetupAttachment(RootComponent);
	TrunkMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 3.f));

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (Cylinder.Succeeded())
	{
		TrunkMesh->SetStaticMesh(Cylinder.Object);
	}

	/**  set trunk material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BrownMaterialTemplate(TEXT("/Game/BP/Blueprints/Pickups/M_Brown"));
	if (BrownMaterialTemplate.Succeeded())
	{
		TrunkMesh->SetMaterial(0, BrownMaterialTemplate.Object);
	}

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cone(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	if (Cone.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(Cone.Object);
	}

	/**  set crown material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialTemplate(TEXT("/Engine/EditorMaterials/WidgetMaterial_Y"));
	if (MaterialTemplate.Succeeded())
	{
		GetStaticMeshComponent()->SetMaterial(0, MaterialTemplate.Object);
	}

	GetStaticMeshComponent()->SetupAttachment(RootComponent);
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(2.75f, 2.75f, 2.5f));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetRelativeLocation(FVector(0.f, 0.f, 250.f));

	/**  set the food class */
	static ConstructorHelpers::FClassFinder<ABaseFood> FoodTemplateClass(TEXT("/Game/BP/Blueprints/Pickups/BP_GoodFood"));
	if (FoodTemplateClass.Class)
	{
		FoodClass = FoodTemplateClass.Class;
	}

	/** AI Perception Stimuli Source Component  */
	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
	
	/** defaults  */
	bShouldSpawn = true;
}

void ATree::BeginPlay()
{
	Super::BeginPlay();

	if (bShouldSpawn)
	{
		FTimerHandle SpawnFoodTimer;
		GetWorldTimerManager().SetTimer(SpawnFoodTimer, this, &ATree::SpawnFood, SpawnFoodDelay, true);
	}
}

void ATree::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AIPerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	AIPerceptionStimuliSource->RegisterWithPerceptionSystem();
}

void ATree::SpawnFood()
{
	if (FoodClass)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator; //  GetController();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			FNavLocation RandomPoint;

			if (World->GetNavigationSystem())
			{
				World->GetNavigationSystem()->GetRandomReachablePointInRadius(TrunkMesh->GetComponentLocation(), SpawnRadius, RandomPoint);
				FVector SpawnLocation = RandomPoint.Location;
				SpawnLocation.Z += FMath::RandRange(50.f, 75.f);

				ABaseFood* Spawned = World->SpawnActor<ABaseFood>(FoodClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
				if (Spawned)
				{
					Spawned->GetFoodMesh()->SetPhysicsAngularVelocityInDegrees(FMath::VRand() * 100000.f);
					Spawned->SetLifeSpan(SpawnFoodDelay * FoodLifeSpan);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FoodClass == NULL"));
	}
}
