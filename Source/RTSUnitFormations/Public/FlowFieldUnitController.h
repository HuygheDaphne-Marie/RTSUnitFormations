// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowFieldTile.h"
#include "UnitController.h"
#include "FlowFieldUnitController.generated.h"

/**
 * 
 */
UCLASS()
class RTSUNITFORMATIONS_API AFlowFieldUnitController : public AUnitController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	virtual void GoToTarget() override;

	UPROPERTY(EditAnywhere)
	AFlowFieldTile* FlowFieldTile{};

	UPROPERTY(EditAnywhere)
	FVector MovementDirection{};
};
