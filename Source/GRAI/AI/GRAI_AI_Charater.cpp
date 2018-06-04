// Fill out your copyright notice in the Description page of Project Settings.

#include "GRAI_AI_Charater.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "AI/UI/StatBlock.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ConstructorHelpers.h"
#include "Engine/DataTable.h"

// Sets default values
AGRAI_AI_Charater::AGRAI_AI_Charater()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	/** removed to HUD  */
	///** stats widget  */
	//Stats = CreateDefaultSubobject<UWidgetComponent>(TEXT("Stats"));
	//Stats->SetupAttachment(RootComponent);
	//Stats->SetWidgetSpace(EWidgetSpace::Screen);
	//Stats->SetDrawSize(FVector2D(450.f, 266.f));

	///** set widget component widget template  */
	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetTemplate(TEXT("/Game/BP/Blueprints/AI/UI/WBP_StatBlock"));
	//if (WidgetTemplate.Class)
	//{
	//	Stats->SetWidgetClass(WidgetTemplate.Class);
	//}

	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	/**  set BP AI controller asset */
	static ConstructorHelpers::FClassFinder<AAIController> AIControllerClassBP(TEXT("/Game/BP/Blueprints/AI/BP_AIC"));
	if (AIControllerClassBP.Class != NULL)
	{
		AIControllerClass = AIControllerClassBP.Class;
	}

	/** set the data table asset  */
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Game/BP/Blueprints/AI/DataTables/DT"));
	if (DataTableAsset.Object)
	{
		StatsDB = DataTableAsset.Object;
	}

	/** set AI movement  */
	GetCharacterMovement()->MaxAcceleration = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);

	// -----------------------------------------------------------------------------------

	/** TODO remove to Data table  */
	/*AIStats = {

	FAIStat(FName("Health"), 80.f, 0.f, 100.f, 1.f, 1.f),
	FAIStat(FName("Stamina"), 60.f, 0.f, 100.f, 1.f, 1.f),
	FAIStat(FName("Hunger"), 60.f, 0.f, 100.f, 1.f, 1.f),
	FAIStat(FName("Strength"), 18.f, 1.f, 25.f, 0.f, 0.f),
	FAIStat(FName("Intelligence"), 8.f, 1.f, 25.f, 0.f, 0.f),
	FAIStat(FName("Speed"), 12.f, 1.f, 25.f, 0.f, 0.f),
	FAIStat(FName("Happiness"), 100.f, -100.f, 100.f, 0.f, 0.f),

	};*/
}

void AGRAI_AI_Charater::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/** removed to HUD  */
	/** set reference in widget component  */
	/*if (UStatBlock* StatBlock = Cast<UStatBlock>(Stats->GetUserWidgetObject()))
	{
		StatBlock->SetOwnerAIPawn(this);
	}*/

	DMI = GetMesh()->CreateDynamicMaterialInstance(0);
}

// Called when the game starts or when spawned
void AGRAI_AI_Charater::BeginPlay()
{
	Super::BeginPlay();

	/** called to fill AIStats array from data table */
	FillAIStats();
	
	/** start the Hungers timer */
	FTimerDelegate HungerTimerDelegate;
	FTimerHandle HungerTimer;
	
	int32 HungerIndex = 2;
	
	HungerTimerDelegate.BindUFunction(this, FName("StatTick"), HungerIndex);
	GetWorldTimerManager().SetTimer(HungerTimer, HungerTimerDelegate, StatTickRate, true);
}

float AGRAI_AI_Charater::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		AIStats[0].CurrentStatValue -= ActualDamage;

		if (AIStats[0].CurrentStatValue <= 0.f)
		{
			/** called to ragdoll the character and stop all timers  */
			KillCharacter();
		}

		/** clamp the health  */
		AIStats[0].CurrentStatValue = FMath::Clamp(AIStats[0].CurrentStatValue, AIStats[0].StatMin, AIStats[0].StatMax);
	}

	return ActualDamage;
}

void AGRAI_AI_Charater::ChangeStamina(float Value)
{
	AIStats[1].CurrentStatValue += Value;
	AIStats[1].CurrentStatValue = FMath::Clamp(AIStats[1].CurrentStatValue, AIStats[1].StatMin, AIStats[1].StatMax);
}

EAIState AGRAI_AI_Charater::CalcAndGetAIState()
{
	if (AIStats[1].CurrentStatValue < 50.f)
	{
		AIState = EAIState::AS_LowStam;
	}
	else if (AIStats[2].CurrentStatValue < 50.f)
	{
		AIState = EAIState::AS_Hungry;
	}
	else
	{
		AIState = EAIState::AS_Wander;
	}

	/** adjust speed  */
	if (AIState == EAIState::AS_Wander)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	} 
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}

	return AIState;
}

void AGRAI_AI_Charater::AI_UseItem_Implementation(class AActor* ItemToUse)
{
	if (ItemToUse && ItemToUse->GetClass()->ImplementsInterface(UUsableInterface::StaticClass()))
	{
		IUsableInterface::Execute_UseItem(ItemToUse, this);

		DialogCallOut(ItemToUse);
	}
}

void AGRAI_AI_Charater::Selected_Implementation()
{
	if (DMI)
	{
		/** save the initial color  */
		FMaterialParameterInfo ParameterInfo("BodyColor");
		DMI->GetVectorParameterValue(ParameterInfo, InitialColor);

		/** change mesh color  */
		DMI->SetVectorParameterValue("BodyColor", FLinearColor::Green);
	}
}

void AGRAI_AI_Charater::Deselected_Implementation()
{
	if (DMI)
	{
		/** change mesh color  */
		DMI->SetVectorParameterValue("BodyColor", InitialColor);
	}
}

int32 AGRAI_AI_Charater::GetMostDesiredStatIndex() const
{
	/*float MostDesiredLevel = FLT_MIN;
	int32 MostDesiredIndex = INDEX_NONE;
	for (int32 StatIndex = StatDesirability.Num() - 1; StatIndex >= 0; --StatIndex)
	{
		if (StatDesirability[StatIndex] > MostDesiredLevel)
		{
			MostDesiredLevel = StatDesirability[StatIndex];
			MostDesiredIndex = StatIndex;
		}
	}

	return MostDesiredIndex;*/
	return 0;
}

void AGRAI_AI_Charater::FillAIStats()
{
	/** read all data table data to array  */
	if (StatsDB)
	{
		FString ContextString;
		TArray<FName> RowNames;
		RowNames = StatsDB->GetRowNames();

		for (FName& RowName : RowNames)
		{
			FAIStat* AIStat = StatsDB->FindRow<FAIStat>(RowName, ContextString);
			if (AIStat)
			{
				AIStats.Add(*AIStat);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StatsDB == NULL!"));
	}
}

void AGRAI_AI_Charater::KillCharacter()
{
	/** clear all timers  */
	GetWorldTimerManager().ClearAllTimersForObject(this);

	/** Ragdoll the character  */
	DetachFromControllerPendingDestroy();

	static FName CollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionProfileName(CollisionProfileName);

	SetActorEnableCollision(true);

	// rag doll  (init Physics)
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	// disable movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	// disable collisions on the capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AGRAI_AI_Charater::StatTick(int32 StatToTick)
{
	/** reduce current stat value  */
	AIStats[StatToTick].CurrentStatValue -= StatTickRate * AIStats[StatToTick].StatDecayRate;

	/** clamp stat  */
	AIStats[StatToTick].CurrentStatValue = FMath::Clamp(AIStats[StatToTick].CurrentStatValue, AIStats[StatToTick].StatMin, AIStats[StatToTick].StatMax);

	if (AIStats[1].CurrentStatValue > 0.f)
	{
		AIStats[0].CurrentStatValue += AIStats[0].StatDecayRate;
		AIStats[0].CurrentStatValue = FMath::Clamp(AIStats[0].CurrentStatValue, AIStats[0].StatMin, AIStats[0].StatMax);
	}

	/** if Hunger is zero - start depleting stamina  */
	if (AIStats[2].CurrentStatValue == 0.f)
	{
		/** if Stamina is zero - start taking damage  */
		if (AIStats[1].CurrentStatValue == 0.f)
		{
			/** decrease our health  */
			UGameplayStatics::ApplyDamage(this, AIStats[0].StatDecayRate, GetController(), this, UDamageType::StaticClass());
		} 
		else
		{
			AIStats[1].CurrentStatValue -= AIStats[1].StatDecayRate;
			AIStats[1].CurrentStatValue = FMath::Clamp(AIStats[1].CurrentStatValue, AIStats[1].StatMin, AIStats[1].StatMax);
		}

		return;
	}

	if (AIStats[2].CurrentStatValue > 0.f)
	{
		AIStats[1].CurrentStatValue += AIStats[1].StatDecayRate;
		AIStats[1].CurrentStatValue = FMath::Clamp(AIStats[1].CurrentStatValue, AIStats[1].StatMin, AIStats[1].StatMax);
	}

}


