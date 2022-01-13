// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARTrackable.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include <vector>
#include "MyActor.generated.h"

UCLASS()
class HOLOLENS_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();
	
	UPROPERTY()
	UARTrackedGeometry *uart;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* orange;

	UPROPERTY(EditAnywhere)
	AActor* whiteKing;

	void OnTrackableUpdated(UARTrackedGeometry* Added);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	std::vector< UMRMeshComponent* > mrMeshVector;
	int nonEmptyMesh = 0;
	int allMesh = 0;
};
