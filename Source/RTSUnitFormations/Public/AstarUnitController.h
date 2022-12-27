// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitController.h"
#include "AstarUnitController.generated.h"

/**
 * 
 */
UCLASS()
class RTSUNITFORMATIONS_API AAstarUnitController : public AUnitController
{
	GENERATED_BODY()

	private:
	virtual void GoToTarget() override;
};
