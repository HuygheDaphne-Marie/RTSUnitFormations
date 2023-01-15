// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldCell.h"

#include "Materials/MaterialInstance.h"

// Sets default values
AFlowFieldCell::AFlowFieldCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root Component");
	SetRootComponent(SceneRoot);
	
	DebugPlane = CreateDefaultSubobject<UStaticMeshComponent>("Debug Plane");
	DebugPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DebugPlane->SetHiddenInGame(true);
	DebugPlane->SetupAttachment(SceneRoot);
	
	// Setup box check visual (not used for any actual collisions)
	CellCollision = CreateDefaultSubobject<UBoxComponent>("Cell Collision");
	CellCollision->SetHiddenInGame(true);
	CellCollision->SetRelativeLocation(FVector{0, 0, 10});
	CellCollision->SetBoxExtent(FVector{50,50,10}, false);
	CellCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CellCollision->SetupAttachment(SceneRoot);

	DesiredDirectionArrow = CreateDefaultSubobject<UArrowComponent>("Direction Arrow");
	DesiredDirectionArrow->SetHiddenInGame(true);
	DesiredDirectionArrow->ArrowSize = 0.5f;
	DesiredDirectionArrow->SetRelativeLocation(FVector{0, 0, 20});
	DesiredDirectionArrow->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AFlowFieldCell::BeginPlay()
{
	Super::BeginPlay();

	const FCollisionShape CheckBox = FCollisionShape::MakeBox(CellCollision->GetScaledBoxExtent());
	bIsWalkable = !GetWorld()->SweepTestByChannel(CellCollision->GetComponentLocation(), CellCollision->GetComponentLocation(),FQuat::Identity,
		ECC_Visibility, CheckBox);
}

void AFlowFieldCell::ToggleShowDebugWalkableStatus()
{
	UMaterialInstance* SelectedMaterial =  bIsWalkable ? WalkableMaterial : NotWalkableMaterial;
	DebugPlane->SetMaterial(0, SelectedMaterial);
	DebugPlane->SetHiddenInGame(!DebugPlane->bHiddenInGame);
}

void AFlowFieldCell::ToggleShowDesiredDirection()
{
	if (bIsShowingDirectionArrowState) // if we're showing and we need to hide
	{
		bIsShowingDirectionArrowState = false;
		DesiredDirectionArrow->SetHiddenInGame(true);
	}
	else // not showing and need to setup display
	{
		bIsShowingDirectionArrowState = true;
		
		if (DesiredMovementDirection.SquaredLength() >= 0.1f)
		{
			DesiredDirectionArrow->SetHiddenInGame(false);
			const FRotator ArrowRotation = DesiredMovementDirection.ToOrientationRotator();
			DesiredDirectionArrow->SetWorldRotation(ArrowRotation);
		}
	}
}

FVector AFlowFieldCell::GetCellExtent() const
{
	if (CellCollision == nullptr)
	{
		return FVector{};
	}
	return CellCollision->GetScaledBoxExtent();
}

void AFlowFieldCell::ResetCell()
{
	IntegrationCost = 7777777;
	DesiredMovementDirection = {0,0,0};
}

void AFlowFieldCell::SetDesiredMovementDirection(FVector Direction)
{
	DesiredMovementDirection = Direction;
	if (bIsShowingDirectionArrowState)
	{
		ToggleShowDesiredDirection();
		ToggleShowDesiredDirection();
	}
}

FVector AFlowFieldCell::GetDesiredMovementDirection() const
{
	return DesiredMovementDirection;
}

