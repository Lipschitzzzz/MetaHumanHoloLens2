// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARBlueprintLibrary.h"
#include "CamCapture.generated.h"

UCLASS()
class HOLOLENS_API ACamCapture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamCapture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMesh* StaticMesh;
	UStaticMeshComponent* StaticMeshComponent;
	UMaterialInstanceDynamic* DynamicMaterial;
	bool IsTextureParamSet = false;


};
