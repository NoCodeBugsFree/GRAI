// Fill out your copyright notice in the Description page of Project Settings.

#include "GRAI_AIC.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Pickups/Tree.h"

AGRAI_AIC::AGRAI_AIC()
{
	/** Blackboard Component  */
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	/** BehaviorTree Component  */
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	/** set the behavior tree asset  */
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeTemplate(TEXT("/Game/BP/Blueprints/AI/BT"));
	if (BehaviorTreeTemplate.Object)
	{
		BehaviorTree = BehaviorTreeTemplate.Object;
	}

	/** AI Perception Component  */
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	/** sight sense config template to our AI  */
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = 2500.f;
	Sight->PeripheralVisionAngleDegrees = 60.f;
	Sight->SetMaxAge(60.f);
	Sight->AutoSuccessRangeFromLastSeenLocation = -1.f;

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*Sight);

	/** hearing sense config template to our AI  */
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	Hearing->HearingRange = 3000.f;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->SetMaxAge(60.f);

	AIPerceptionComponent->ConfigureSense(*Hearing);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

void AGRAI_AIC::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	// Initialize Blackboard and Start Behavior Tree 
	if (Pawn && BehaviorTree && BehaviorTree->BlackboardAsset)
	{
		BlackboardComponent->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

	/** perception delegates  */
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AGRAI_AIC::OnPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGRAI_AIC::OnTargetPerceptionUpdated);
}

void AGRAI_AIC::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{

	}
}

void AGRAI_AIC::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	TSubclassOf<UAISense> TempSens = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);

	if (Actor && Actor->IsA(ATree::StaticClass()))
	{
		if(ATree* Tree = Cast<ATree>(Actor))
		{
			Trees.AddUnique(Tree);
		}
	}
}

