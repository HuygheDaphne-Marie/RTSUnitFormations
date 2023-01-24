// Fill out your copyright notice in the Description page of Project Settings.


#include "DataFileWriter.h"

#include <chrono>
#include <fstream>
#include <string>

// Sets default values for this component's properties
UDataFileWriter::UDataFileWriter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDataFileWriter::BeginPlay()
{
	Super::BeginPlay();
}

void UDataFileWriter::StartCounter()
{
	MsStart = std::chrono::duration_cast<std::chrono::milliseconds>(
std::chrono::system_clock::now().time_since_epoch());
}

void UDataFileWriter::StopCounter()
{
	MsEnd = std::chrono::duration_cast<std::chrono::milliseconds>(
std::chrono::system_clock::now().time_since_epoch());
}

void UDataFileWriter::WriteDurationToFile(FString Filename, FString DataPrefix)
{
	std::chrono::milliseconds MsDuration = std::chrono::duration_cast<std::chrono::milliseconds>(MsEnd - MsStart);
	std::ofstream FlowFieldOutputFile;
	FString FileLocation = "C:/Unreal/Projects/RTSUnitFormations/";
	FlowFieldOutputFile.open(std::string(TCHAR_TO_UTF8(*FileLocation)) + std::string(TCHAR_TO_UTF8(*Filename)), std::ios::out | std::ios::app);
	FlowFieldOutputFile << std::string(TCHAR_TO_UTF8(*DataPrefix)) << "," << std::to_string(MsDuration.count()) << std::endl;
}



