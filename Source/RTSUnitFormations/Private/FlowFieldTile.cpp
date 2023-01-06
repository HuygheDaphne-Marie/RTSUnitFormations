// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldTile.h"

// Sets default values
AFlowFieldTile::AFlowFieldTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlowFieldTile::BeginPlay()
{
	Super::BeginPlay();
	Cells.Reserve(GridCols * GridRows);
	if (CellClass == nullptr)
		CellClass = AFlowFieldCell::StaticClass();
	
	SpawnCells();
}

void AFlowFieldTile::SpawnCells()
{
	const FVector StartSpawnPos = GetActorLocation() + FVector{50,50,0}; // Shitty magic value
	FVector SpawnPos = StartSpawnPos;

	// Cell Step
	bool bIsCellStepSet = false;
	FVector CellStep;

	for (int Row = 0; Row < GridRows; Row++)
	{
		for (int Col = 0; Col < GridCols; Col++)
		{
			AFlowFieldCell* Cell = Cast<AFlowFieldCell>(GetWorld()->SpawnActor(CellClass, &SpawnPos));
			Cells.Add(Cell);
			
			if (!bIsCellStepSet)
			{
				CellStep = Cell->GetCellExtent() * 2;
				bIsCellStepSet = true;

				GridExtents.X = GridCols * CellStep.X;
				GridExtents.Y = GridRows * CellStep.Y;
			}
			
			SpawnPos.X += CellStep.X;
		}

		SpawnPos.X = StartSpawnPos.X;
		SpawnPos.Z = StartSpawnPos.Z;

		SpawnPos.Y += CellStep.Y;
	}
}

// Called every frame
void AFlowFieldTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

