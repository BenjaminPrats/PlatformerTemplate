// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralPlatform.h"
#include "Public/Misc/FileHelper.h"
#include "Public/HAL/PlatformFileManager.h"
#include "Public/GenericPlatform/GenericPlatformFile.h"

// Sets default values
AProceduralPlatform::AProceduralPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;

#if 1 // TODO: (Benjamin) https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
	// New in UE 4.17, multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
#endif
}

// Called when the game starts or when spawned
void AProceduralPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void AProceduralPlatform::PostActorCreated()
{
	Super::PostActorCreated();
	// CreateTriangle();
	CreatePolygon();
}

// This is called when actor is already in level and map is opened
void AProceduralPlatform::PostLoad()
{
	Super::PostLoad();
	CreatePolygon();
}

void AProceduralPlatform::CreateTriangle()
{
	TArray<FVector> vertices;
	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(0, 0, 100));

	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	TArray<FVector> normals;
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));


	TArray<FProcMeshTangent> tangents;
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	TArray<FLinearColor> vertexColors;
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void AProceduralPlatform::GetPolygonFromFile(const FString& fileName, TArray<FVector>& vertices, TArray<int32>& polygons)
{
	FString saveDirectory = FPaths::GameSourceDir() + FString("../_externalAsset/Polygons");
	FString textToSave = FString("Lorem ipsum");
	bool allowOverwriting = false;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*saveDirectory))
	{
		// Get relative file path
		FString relativeFilePath = saveDirectory + "/" + fileName;
		UE_LOG(LogTemp, Warning, TEXT("Sucess1"));
		// Allow overwriting or file doesn't already exist
		if (allowOverwriting || !FPlatformFileManager::Get().GetPlatformFile().FileExists(*relativeFilePath))
		{
			FFileHelper::SaveStringToFile(textToSave, *relativeFilePath);
			UE_LOG(LogTemp, Warning, TEXT("Sucess2"));
		}
	}
}

void AProceduralPlatform::CreatePolygon()
{
	TArray<FVector> vertices;
	TArray<int32> polygons;
	FString fileName("testUnreal");

	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(0, 0, 100));
	vertices.Add(FVector(0, 100, 100));

	GetPolygonFromFile(fileName, vertices, polygons);

}