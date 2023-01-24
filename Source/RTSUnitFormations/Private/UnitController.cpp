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
	{
		// Target = Targets[0];
		SetTarget(Targets[0]->GetTransform().GetLocation());
	}

	// Stuff suddenly broke oh no
	// FTimerHandle UnusedHandle;
	// GetWorld()->GetTimerManager().SetTimer(
	// 	UnusedHandle, this, &AUnitController::BeginControl, 2.0f, false);
}

void AUnitController::BeginControl()
{

	// Logging should happen here so all unit controllers do it the same way

	// Find & set target
	TArray<AActor*> Targets{};
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Target", Targets);
	if (!Targets.IsEmpty())
	{
		// Target = Targets[0];
		SetTarget(Targets[0]->GetTransform().GetLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Ready for control!"))
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
