// Fill out your copyright notice in the Description page of Project Settings.


#include "AstarUnitController.h"

void AAstarUnitController::GoToTarget()
{
	Super::GoToTarget();

	// UE_LOG(LogTemp, Warning, TEXT("Going to target!"))
	MoveToLocation(TargetPos, -1, true, true);
}
