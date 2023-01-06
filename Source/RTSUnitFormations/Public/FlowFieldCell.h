// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void ToggleShowDebugWalkableStatus() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCellExtent() const;

	// Cell State
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWalkable = false;
	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CellCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DebugPlane;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;

	// Debug Visualisation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* WalkableMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* NotWalkableMaterial;
};
