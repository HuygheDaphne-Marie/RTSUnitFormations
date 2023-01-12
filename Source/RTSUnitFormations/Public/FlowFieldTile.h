// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowFieldCell.h"
#include "GameFramework/Actor.h"
#include "FlowFieldTile.generated.h"

UCLASS()
class RTSUNITFORMATIONS_API AFlowFieldTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlowFieldTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SpawnCells();

public:
	UFUNCTION(BlueprintCallable)
	void CalculateFlowField(FVector TargetWorldLocation);
	

	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFlowFieldCell> CellClass = AFlowFieldCell::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridCols = 10; // X

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridRows = 10; // Y

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GridExtents{0,0,200};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AFlowFieldCell*> Cells{};

private:
	UFUNCTION(BlueprintCallable)
	void GenerateIntegrationField(FVector TargetWorldLocation);
	UFUNCTION(BlueprintCallable)
	void GenerateFlowField();
	
	// Helpers
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AFlowFieldCell* GetCellFromWorldPos(FVector Position) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCellIndex(AFlowFieldCell* Cell) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AFlowFieldCell*> GetCellNeighbors(int Index, bool bReturnOnlyWalkable = true) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntVector2 IndexToCoordinate(int Index) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int CoordinateToIndex(FIntVector2 Coordinate) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCoordinateValid(FIntVector2 Coordinate) const;

	UPROPERTY()
	const FVector CellSize{100,100,0}; // Shitty magic value
};
