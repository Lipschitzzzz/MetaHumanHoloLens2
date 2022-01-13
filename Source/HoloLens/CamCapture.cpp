// Fill out your copyright notice in the Description page of Project Settings.


#include "CamCapture.h"

// Sets default values
ACamCapture::ACamCapture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Load a mesh from the engine to render the camera feed to.
    StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EngineMeshes/Cube.Cube"), nullptr, LOAD_None, nullptr);

    // Create a static mesh component to render the static mesh
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraPlane"));
    StaticMeshComponent->SetStaticMesh(StaticMesh);

    // Scale and add to the scene
    StaticMeshComponent->SetWorldScale3D(FVector(0.1f, 1, 1));
    this->SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ACamCapture::BeginPlay()
{
	Super::BeginPlay();
	
    // Create a dynamic material instance from the game's camera material.
    // Right-click on a material in the project and select "Copy Reference" to get this string.
    
    FString CameraMatPath("Material'/Game/Assets/Materials/CamTextureMaterial.CamTextureMaterial'");
    UMaterial* BaseMaterial = (UMaterial*)StaticLoadObject(UMaterial::StaticClass(), nullptr, *CameraMatPath, nullptr, LOAD_None, nullptr);
    DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

    // Use the dynamic material instance when rendering the camera mesh.
    StaticMeshComponent->SetMaterial(0, DynamicMaterial);

    // Start the webcam.
    bool a = UARBlueprintLibrary::ToggleARCapture(true, EARCaptureType::SpatialMapping);
    UE_LOG(LogTemp, Warning, TEXT("%d"), a);

}

// Called every frame
void ACamCapture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Dynamic material instance only needs to be set once.
    if (IsTextureParamSet)
    {
        UE_LOG(LogTemp, Warning, TEXT("ARTexture1"));

        return;
    }

    // Get the texture from the camera.
    UARTexture* ARTexture = UARBlueprintLibrary::GetARTexture(EARTextureType::CameraImage);
    UE_LOG(LogTemp, Warning, TEXT("%d"), ARTexture == nullptr);

    if (ARTexture != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("ARTexture3"));

        // Set the shader's texture parameter (named "Param") to the camera image.
        DynamicMaterial->SetTextureParameterValue("Param", ARTexture);
        IsTextureParamSet = true;

        // Get the camera instrincs
        FARCameraIntrinsics Intrinsics;
        UARBlueprintLibrary::GetCameraIntrinsics(Intrinsics);

        // Scale the camera mesh by the aspect ratio.
        float R = (float)Intrinsics.ImageResolution.X / (float)Intrinsics.ImageResolution.Y;
        StaticMeshComponent->SetWorldScale3D(FVector(0.1f, R, 1));
    }

}

