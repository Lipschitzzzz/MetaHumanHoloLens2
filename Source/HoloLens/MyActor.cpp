// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "ARTrackable.h"
#include "ARBlueprintLibrary.h"
#include "MRMeshComponent.h"


// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	uart = NewObject<UARTrackedGeometry>();
	// Subscribe to Tracked Geometry delegates
	UARBlueprintLibrary::AddOnTrackableUpdatedDelegate_Handle(
		FOnTrackableAddedDelegate::CreateUObject(this, &AMyActor::OnTrackableUpdated)
	);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	meshIndex = 0;
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("TrackingState: %f"), uart->GetObjectClassification()));
}

void AMyActor::OnTrackableUpdated(UARTrackedGeometry* Added)
{
	/*if (Added->GetObjectClassification() != EARObjectClassification::World)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), Added->GetObjectClassification()));
	}*/
	// When tracked geometry is received, check that it's from spatial mapping
	if (Added->GetObjectClassification() == EARObjectClassification::World)
	{
		UMRMeshComponent* MRMesh = Added->GetUnderlyingMesh();
		

		MRMesh->SetMaterial(0, Orange);
		FVector mrPosition = MRMesh->GetComponentLocation();
		 /*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(
				TEXT("Position x: %f, Position y: %f, Position z: %f"), mrPosition.X, mrPosition.Y, mrPosition.Z));
				*/
	}
}

