// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/ActorComponent.h"
#include "ARTrackable.h"
#include "ARBlueprintLibrary.h"
#include "MRMeshComponent.h"
#include "PhysicsPublic.h"
#include "PhysXIncludes.h"
#include <Interface_CollisionDataProviderCore.h>


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
	// uart = NewObject<UARTrackedGeometry>();
	// Subscribe to Tracked Geometry delegates
	UARBlueprintLibrary::AddOnTrackableUpdatedDelegate_Handle(
		FOnTrackableAddedDelegate::CreateUObject(this, &AMyActor::OnTrackableUpdated)
	);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// meshIndex = 0;
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Numbers of MRMesh: %d"), mrVector.size()));
	// UE_LOG(LogTemp, Warning, TEXT("Numbers of MRMesh: %d"), mrVector.size());

}

void AMyActor::OnTrackableUpdated(UARTrackedGeometry* Added)
{
	// trz to access the camera then calculate distance but didn't work so I changed to whiteKing
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector camLocation = camManager->GetCameraLocation();
	FVector camForward = camManager->GetCameraRotation().Vector();
	
	// Everytime when call this func the UARTrackedGeometry class must be the World
	// Don't understand what exactly this class is
	// In which case we could get the other type?
	if (Added->GetObjectClassification() != EARObjectClassification::World)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Classification: %d"), Added->GetObjectClassification()));
			
	}

	// whiteKingLocation
	FVector whiteKingLocation = whiteKing->GetActorLocation();
	// UE_LOG(LogTemp, Warning, TEXT("x: %f,y: %f,z: %f"), whiteKingLocation.X, whiteKingLocation.Y, whiteKingLocation.Z);
	


	// When tracked geometry is received, check that it's from spatial mapping
	if (Added->GetObjectClassification() == EARObjectClassification::World)
	{
		UMRMeshComponent* MRMesh = Added->GetUnderlyingMesh();
		if (MRMesh)
		{
			//MRMesh->SetMaterial(allMesh, orange);
			FVector mrPosition = MRMesh->GetComponentLocation();

			// float distance = (mrPosition - whiteKingLocation).Size();
			// UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distance);
			// UE_LOG(LogTemp, Warning, TEXT("Position x: %f, Position y: %f, Position z: %f"), mrPosition.X, mrPosition.Y, mrPosition.Z);

			// Always print the first element to see how it changes
			
			//UE_LOG(LogTemp, Warning, TEXT("Non-Empty Mesh Numbers: %d"), nonEmptyMesh);
			//UE_LOG(LogTemp, Warning, TEXT("All Mesh Numbers: %d"), allMesh);

			// if the MRMesh is not empty
			if (mrPosition.X != 0.0f && mrPosition.Y != 0.0f && mrPosition.Z != 0.0f)
			{
				// MRMesh->SetMaterial(allMesh, orange);
				nonEmptyMesh += 1;
				// UE_LOG(LogTemp, Warning, TEXT("Non-Empty Mesh Position x: %f, Non-Empty Mesh Position y: %f, Non-Empty Mesh Position z: %f"),
					//mrPosition.X, mrPosition.Y, mrPosition.Z);
				mrMeshVector.push_back(MRMesh);

				/* 
				source code
				bool UMRMeshComponent::GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData)
				{
					if (TempPosition && TempIndices)
					{
						// Copy the vertices
						CollisionData->Vertices = *TempPosition;

						// Copy the indices
						const auto& Indices = *TempIndices;
						CollisionData->Indices.Reset(Indices.Num() / 3);
						for (auto Index = 0; Index < Indices.Num(); Index += 3)
						{
							FTriIndices Face;
							Face.v0 = Indices[Index];
							Face.v1 = Indices[Index + 1];
							Face.v2 = Indices[Index + 2];
							CollisionData->Indices.Add(Face);
						}

						return true;
					}

					return false;
				}
				*/

				FTriMeshCollisionData* CollisionData{};
				

				// source code
				/** Temporarily saved data pointers used for constructing the collision mesh */
				// TArray<FVector>* TempPosition = nullptr;
				// TArray<MRMESH_INDEX_TYPE>* TempIndices = nullptr;
				
				int nonempty = 0;
				int b = 3;
				if (MRMesh->TempPosition && MRMesh->TempIndices)
				{

					TArray<FVector>* TFV = MRMesh->TempPosition;
					/*
					TArray<uint16>* TFU = MRMesh->TempIndices;
					//UE_LOG(LogTemp, Warning, TEXT("%d"), b);
					//UE_LOG(LogTemp, Warning, TEXT("TFV->Num() %d"), TFV->Num());
					//UE_LOG(LogTemp, Warning, TEXT("TFU->Num() %d"), TFU->Num());

					//UE_LOG(LogTemp, Warning, TEXT("Vertices %s"), );
					for (size_t i = 0; i < 0; i++)
					{
						if (TFU->GetData()[i] != 32760 && TFU->GetData()[i] != 0)
						{
							UE_LOG(LogTemp, Warning, TEXT("i: %d "), i);
							UE_LOG(LogTemp, Warning, TEXT("TFU->GetData()[i].X %d"), TFU->GetData()[i]);
							nonempty += 1;

						}
						//UE_LOG(LogTemp, Warning, TEXT("TFV->GetData()[i].X %f"), TFV->GetData()[i].X);
						//UE_LOG(LogTemp, Warning, TEXT("TFU->GetData()[i].X %d"), TFU->GetData()[i]);

					}
					*/
					b = 1;
					// UE_LOG(LogTemp, Warning, TEXT("nonempty %d"), nonempty);

				}
				
				//UE_LOG(LogTemp, Warning, TEXT("b: %d"), b);


			}

			allMesh += 1;

			// For long-term saving
			//UE_LOG(LogTemp, Warning, TEXT("size of vector %d"), mrMeshVector.size());
			
			if (mrMeshVector.size() > 0)
			{
				FVector firstElementPosition = mrMeshVector.front()->GetComponentLocation();
				//UE_LOG(LogTemp, Warning, TEXT("firstElementPosition x: %f, firstElementPosition y: %f, firstElementPosition z: %f"), firstElementPosition.X, firstElementPosition.Y, firstElementPosition.Z);
			}


		}
			
		// it does not work either.
		// MRMesh->SetEnableMeshOcclusion(true);

		// PxTriangleMesh* TempTriMesh = MRMesh->GetBodySetup()->TriMeshes[0];

		//UE_LOG(LogTemp, Warning, TEXT("bodyinstances: %d"), MRMesh->BodyInstances.Num());

		if (MRMesh->BodyInstances.Num() != 0)
		{
			// UE_LOG(LogTemp, Warning, TEXT("BodyInstances is non empty"));

			// UE_LOG(LogTemp, Warning, TEXT("bodyinstances: %d"), MRMesh->BodyInstance.BodySetup.Get());
			// UE_LOG(LogTemp, Warning, TEXT("bodyinstances: %s"), MRMesh->BodyInstance.BodySetup.Get()->GetClass());
			// UE_LOG(LogTemp, Warning, TEXT("bodyinstances: %s"), MRMesh->BodyInstance.BodySetup.Get()->TriMeshes);

			
			TArray<UStaticMeshComponent*> Components;
			whiteKing->GetComponents<UStaticMeshComponent>(Components);
			// UE_LOG(LogTemp, Warning, TEXT("whiteKinginstances: %d"), Components[0]->BodyInstance.BodySetup.Get());
		}

		// the issue is here. Get return empty
		


		/*
		for (int32 i = 0; i < Components.Num(); i++)
		{
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			PxTriangleMesh* TempTriMesh = StaticMeshComponent->BodyInstance.BodySetup.Get()->TriMeshes[0];

			check(TempTriMesh);
			int32 TriNumber = TempTriMesh->getNbTriangles();

			const PxVec3* PVertices = TempTriMesh->getVertices();
			const void* Triangles = TempTriMesh->getTriangles();

			// Grab triangle indices
			int32 I0, I1, I2;

			for (int32 TriIndex = 0; TriIndex < TriNumber; ++TriIndex)
			{
				if (TempTriMesh->getTriangleMeshFlags() && PxTriangleMeshFlag::e16_BIT_INDICES)
				{
					PxU16* P16BitIndices = (PxU16*)Triangles;
					I0 = P16BitIndices[(TriIndex * 3) + 0];
					I1 = P16BitIndices[(TriIndex * 3) + 1];
					I2 = P16BitIndices[(TriIndex * 3) + 2];
				}
				else
				{
					PxU32* P32BitIndices = (PxU32*)Triangles;
					I0 = P32BitIndices[(TriIndex * 3) + 0];
					I1 = P32BitIndices[(TriIndex * 3) + 1];
					I2 = P32BitIndices[(TriIndex * 3) + 2];
				}

				UE_LOG(LogTemp, Warning, TEXT("I0: %d, I1: %d, I2: %d"), I0, I1, I2);

				
				float V0 = PVertices->x;
				UE_LOG(LogTemp, Warning, TEXT("V0: %f"), V0);

			}

			// Local position


			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
		}


		//UStaticMeshComponent* s{  };
		//PxTriangleMesh* TempTriMesh = MRMesh->BodyInstance.BodySetup.Get()->TriMeshes[0];
		
		
		//check(TempTriMesh);
		int32 TriNumber = TempTriMesh->getNbTriangles();

		const PxVec3* PVertices = TempTriMesh->getVertices();
		const void* Triangles = TempTriMesh->getTriangles();

		// Grab triangle indices
		int32 I0, I1, I2;
		
		for (int32 TriIndex = 0; TriIndex < TriNumber; ++TriIndex)
		{
			if (TempTriMesh->getTriangleMeshFlags() && PxTriangleMeshFlag::e16_BIT_INDICES)
			{
				PxU16* P16BitIndices = (PxU16*)Triangles;
				I0 = P16BitIndices[(TriIndex * 3) + 0];
				I1 = P16BitIndices[(TriIndex * 3) + 1];
				I2 = P16BitIndices[(TriIndex * 3) + 2];
			}
			else
			{
				PxU32* P32BitIndices = (PxU32*)Triangles;
				I0 = P32BitIndices[(TriIndex * 3) + 0];
				I1 = P32BitIndices[(TriIndex * 3) + 1];
				I2 = P32BitIndices[(TriIndex * 3) + 2];
			}

			UE_LOG(LogTemp, Warning, TEXT("I0: %d, I1: %d, I2: %d"), I0, I1, I2);

		}
		*/




	}

}

