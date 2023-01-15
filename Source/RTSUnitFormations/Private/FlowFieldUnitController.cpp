// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldUnitController.h"

#include "FlowFieldTile.h"
#include "Kismet/GameplayStatics.h"

void AFlowFieldUnitController::BeginPlay()
{
	Super::BeginPlay();

	// get the flowfield
	FlowFieldTile = Cast<AFlowFieldTile>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlowFieldTile::StaticClass()));
}

void AFlowFieldUnitController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FlowFieldTile == nullptr)
		return;

	AFlowFieldCell* OccupiedCell = FlowFieldTile->GetCellFromWorldPos(GetPawn()->GetActorLocation());
	if (OccupiedCell->bIsWalkable)
		MovementDirection = OccupiedCell->GetDesiredMovementDirection();

	if (MovementDirection.SquaredLength() >= 0.1f)
		GetPawn()->AddMovementInput(MovementDirection);
	
}

void AFlowFieldUnitController::GoToTarget()
{
	Super::GoToTarget();

	
}
