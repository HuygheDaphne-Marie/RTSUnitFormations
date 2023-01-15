// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FlowFieldCell.generated.h"

UCLASS()
class RTSUNITFORMATIONS_API AFlowFieldCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlowFieldCell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void ToggleShowDebugWalkableStatus();

	UFUNCTION(BlueprintCallable)
	void ToggleShowDesiredDirection();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCellExtent() const;

	// Cell State
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWalkable = false;

	char Cost = 1; // every cell has a cost of 1, rougher can be more, impassable is 255, target is 0
	int IntegrationCost = 7777777; // the best cost of a cell
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DesiredMovementDirection{0,0,0};
	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CellCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DebugPlane;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* DesiredDirectionArrow;
	
	// Debug Visualisation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* WalkableMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* NotWalkableMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsShowingDirectionArrowState = false;
};
