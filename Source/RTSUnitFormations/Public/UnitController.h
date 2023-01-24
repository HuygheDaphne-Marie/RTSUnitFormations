// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitController.generated.h"

/**
 * Abstract controller class, meant to allow for easy implementation of different pathfinding strategies
 * while providing a place for unified/shared functionality
 *
 * Should handle logging & perhaps performance (some) measurements as well
 */
UCLASS(Abstract)
class RTSUNITFORMATIONS_API AUnitController : public AAIController
{
	GENERATED_BODY()
public:
	AUnitController();

protected:
	virtual void BeginPlay() override;

	virtual void BeginControl();
	
	UFUNCTION(BlueprintCallable)
	virtual void GoToTarget();

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	FVector TargetPos;
	
public:
	void SetTarget(const FVector& worldPos);
};
