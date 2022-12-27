// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitController.h"

#include "Kismet/GameplayStatics.h"

AUnitController::AUnitController()
{
	bStartAILogicOnPossess = true;
}

void AUnitController::BeginPlay()
{
	Super::BeginPlay();

	// Logging should happen here so all unit controllers do it the same way

	// Find & set target
	TArray<AActor*> Targets{};
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Target", Targets);
	if (!Targets.IsEmpty())
		SetTarget(Targets[0]->GetTransform().GetLocation());

	//GoToTarget();
}

void AUnitController::GoToTarget()
{
	// Pathfind towards set target
		// Implemented in non abstract subclasses
}

void AUnitController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void AUnitController::SetTarget(const FVector& worldPos)
{
	TargetPos = worldPos;
}
