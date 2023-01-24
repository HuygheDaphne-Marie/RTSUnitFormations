// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <chrono>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataFileWriter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSUNITFORMATIONS_API UDataFileWriter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDataFileWriter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartCounter();

	UFUNCTION(BlueprintCallable)
	void StopCounter();

	UFUNCTION(BlueprintCallable)
	void WriteDurationToFile(FString Filename, FString DataPrefix);

private:
	std::chrono::milliseconds MsStart;
	std::chrono::milliseconds MsEnd;
};
