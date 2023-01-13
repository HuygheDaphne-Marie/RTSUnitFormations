// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldTile.h"

// Sets default values
AFlowFieldTile::AFlowFieldTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	const FVector StartSpawnPos = GetActorLocation() + CellSize / 2;
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

void AFlowFieldTile::CalculateFlowField(FVector TargetWorldLocation)
{
}

void AFlowFieldTile::GenerateIntegrationField(FVector TargetWorldLocation)
{
	// find if target is in flow field bounds
	AFlowFieldCell* TargetCell = GetCellFromWorldPos(TargetWorldLocation);
	if(TargetCell == nullptr || !TargetCell->bIsWalkable)
		return;
	TargetCell->IntegrationCost = 0;
	
	TQueue<AFlowFieldCell*> OpenCells{};
	OpenCells.Enqueue(TargetCell);

	// Creation of Integration cost field
	while (!OpenCells.IsEmpty())
	{
		AFlowFieldCell* CurrentCell{};
		OpenCells.Dequeue(CurrentCell);

		TArray<AFlowFieldCell*> Neighbors = GetCellNeighbors(GetCellIndex(CurrentCell));
		for (AFlowFieldCell* Neighbor : Neighbors)
		{
			const int NewBestCost = Neighbor->Cost + CurrentCell->IntegrationCost;
			if (NewBestCost < Neighbor->IntegrationCost)
			{
				Neighbor->IntegrationCost = NewBestCost;
				OpenCells.Enqueue(Neighbor);
			}
		}
	}
}

void AFlowFieldTile::GenerateFlowField()
{
	for (AFlowFieldCell* Cell : Cells)
	{
		int LowestCost = Cell->IntegrationCost;
		AFlowFieldCell* BestNeighbor = nullptr;
		
		TArray<AFlowFieldCell*> Neighbors = GetCellNeighbors(GetCellIndex(Cell));
		for (AFlowFieldCell* Neighbor : Neighbors)
		{
			if (Neighbor->IntegrationCost < LowestCost)
			{
				BestNeighbor = Neighbor;
				LowestCost = Neighbor->IntegrationCost;
			}
		}

		if (BestNeighbor != nullptr) // Will be nullptr if it's the destination
		{
			const FIntPoint OriginCoordinate = IndexToCoordinate(GetCellIndex(Cell));
		    const FIntPoint TargetCoordinate = IndexToCoordinate(GetCellIndex(BestNeighbor));

			const FIntPoint TravelDirection{TargetCoordinate.X - OriginCoordinate.X, TargetCoordinate.Y - OriginCoordinate.Y};

			Cell->DesiredMovementDirection.X = TravelDirection.X;
			Cell->DesiredMovementDirection.Y = TravelDirection.Y;
		}
		else
		{
			// set desired travel direction to none
			Cell->DesiredMovementDirection.X = 0;
			Cell->DesiredMovementDirection.Y = 0;
		}
	}
}

// Helpers
AFlowFieldCell* AFlowFieldTile::GetCellFromWorldPos(const FVector Position) const
{
	const FVector FlowFieldOrigin = GetActorLocation();
	const FVector FlowFieldExtent = FlowFieldOrigin + GridExtents;
	if (Position.X < FlowFieldOrigin.X || Position.Y < FlowFieldOrigin.Y)
		return nullptr;
	if (Position.X > FlowFieldExtent.X || Position.Y > FlowFieldExtent.Y)
		return nullptr;

	// Position is in grid
	const FVector OriginToPos = Position - FlowFieldOrigin;

	const FIntPoint CellCoord
	{
		UE4::SSE::FloorToInt32(OriginToPos.X / CellSize.X),
		UE4::SSE::FloorToInt32(OriginToPos.Y / CellSize.Y)
	};

	const int CellIndex = CoordinateToIndex(CellCoord);
	if (Cells.IsValidIndex(CellIndex))
		return Cells[CellIndex];
	
	return nullptr;
}

int AFlowFieldTile::GetCellIndex(AFlowFieldCell* Cell) const
{
	return Cells.Find(Cell); 
}

TArray<AFlowFieldCell*> AFlowFieldTile::GetCellNeighbors(const int Index, const bool bReturnOnlyWalkable) const
{
	TArray<AFlowFieldCell*> Neighbors{};
	if (!Cells.IsValidIndex(Index))
		return Neighbors;

	FIntPoint CellCoordinate = IndexToCoordinate(Index);
	// Right
	CellCoordinate.X++;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);
	// Top Right
	CellCoordinate.Y++;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);
	// Bottom Right
	CellCoordinate.Y -= 2;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);

	// Reset Y to middle
	CellCoordinate.Y++;

	// Left
	CellCoordinate.X -= 2;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);
	// Top Left
	CellCoordinate.Y++;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);
	// Bottom Left
	CellCoordinate.Y -= 2;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);


	// reset X & Y to middle
	CellCoordinate.X++;
	CellCoordinate.Y++;

	// Top
	CellCoordinate.Y++;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);
	// Bottom
	CellCoordinate.Y -= 2;
	if (IsCoordinateValid(CellCoordinate))
		if (bReturnOnlyWalkable && Cells[CoordinateToIndex(CellCoordinate)]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[CoordinateToIndex(CellCoordinate)]);

	return Neighbors;
}

FIntPoint AFlowFieldTile::IndexToCoordinate(const int Index) const
{
	return FIntPoint{Index % GridCols, Index / GridCols};
}

int AFlowFieldTile::CoordinateToIndex(const FIntPoint Coordinate) const
{
	return Coordinate.Y * GridCols + Coordinate.X;
}

bool AFlowFieldTile::IsCoordinateValid(const FIntPoint Coordinate) const
{
	if (Coordinate.X < 0 || Coordinate.Y < 0)
		return false;
	if (Coordinate.X > GridCols || Coordinate.Y > GridRows)
		return false;

	return true;
}
