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

void AFlowFieldTile::CalculateFlowField_Implementation(FVector TargetWorldLocation)
{
	if (bIsDirty)
		ResetField();
	
	GenerateIntegrationField(TargetWorldLocation);
	GenerateFlowField();
	bIsDirty = true;
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
		if (!Cell->bIsWalkable)
			continue;
		
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

			Cell->SetDesiredMovementDirection(FVector{static_cast<double>(TravelDirection.X), static_cast<double>(TravelDirection.Y), 0});
		}
		else
		{
			// set desired travel direction to none
			Cell->SetDesiredMovementDirection(FVector{0, 0, 0});
		}
	}
}

void AFlowFieldTile::ResetField()
{
	for (AFlowFieldCell* Cell : Cells)
		Cell->ResetCell();

	bIsDirty = false;
	// UE_LOG(LogTemp, Warning, TEXT("Cells reset!"))
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

	// might be better to calculate the neighboring indices, not using coords
	// left is -1 right is +1
	// row above is + gridwith
	// row below is - gridwith
	// same for left right of those

	// Right
	if (Cells.IsValidIndex(Index + 1))
		if (bReturnOnlyWalkable && Cells[Index + 1]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[Index + 1]);
	// Left
	if (Cells.IsValidIndex(Index - 1))
		if (bReturnOnlyWalkable && Cells[Index - 1]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[Index - 1]);

	// Above
	const int AboveIndex = Index + GridCols;
	if (Cells.IsValidIndex(AboveIndex)) // if the above doesn't exist neither will it's left or right
	{
		if (bReturnOnlyWalkable && Cells[AboveIndex]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[AboveIndex]);

		// Right
		if (Cells.IsValidIndex(AboveIndex + 1))
			if (bReturnOnlyWalkable && Cells[AboveIndex + 1]->bIsWalkable || !bReturnOnlyWalkable)
				Neighbors.Add(Cells[AboveIndex + 1]);
		// Left
		if (Cells.IsValidIndex(AboveIndex - 1))
			if (bReturnOnlyWalkable && Cells[AboveIndex - 1]->bIsWalkable || !bReturnOnlyWalkable)
				Neighbors.Add(Cells[AboveIndex - 1]);
	}

	// Below
	const int BelowIndex = Index - GridCols;
	if (Cells.IsValidIndex(BelowIndex)) // if the below doesn't exist neither will it's left or right
	{
		if (bReturnOnlyWalkable && Cells[BelowIndex]->bIsWalkable || !bReturnOnlyWalkable)
			Neighbors.Add(Cells[BelowIndex]);

		// Right
		if (Cells.IsValidIndex(BelowIndex + 1))
			if (bReturnOnlyWalkable && Cells[BelowIndex + 1]->bIsWalkable || !bReturnOnlyWalkable)
				Neighbors.Add(Cells[BelowIndex + 1]);
		// Left
		if (Cells.IsValidIndex(BelowIndex - 1))
			if (bReturnOnlyWalkable && Cells[BelowIndex - 1]->bIsWalkable || !bReturnOnlyWalkable)
				Neighbors.Add(Cells[BelowIndex - 1]);
	}

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
