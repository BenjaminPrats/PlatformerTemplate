// Fill out your copyright notice in the Description page of Project Settings.

#define REAL double // for the Triangle library

#include "ProceduralPlatform.h"

#include "../_externalLibs/TrianglePP/Includes/tpp_interface.hpp"
#include "../_externalLibs/TrianglePP/Includes/dpoint.hpp"
//#include "tpp_interface.hpp"


#include "Runtime/Core/Public/Misc/CString.h"
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

void AProceduralPlatform::GetPolygonFromFile(const FString& fileName, TArray<FVector>& vertices, TArray<int32>& triangles)
{
	FString saveDirectory = FPaths::GameSourceDir() + FString("../_externalAsset/Polygons");

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*saveDirectory))
	{
		// Get relative file path
		FString relativeFilePath = saveDirectory + "/" + fileName;
		// Allow overwriting or file doesn't already exist
		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*relativeFilePath))
		{
			// V_LOG(LogTemp, "The file doesn't exist!");
			return;
		}

		TArray<FString> lines;
		FFileHelper::LoadFileToStringArray(lines, *relativeFilePath);
		const int32 startingLineOutsideVertices = 2;
		const int32 nbOutsideVertices = FCString::Atoi(*lines[startingLineOutsideVertices - 1]);
		const int32 startingLineInsideVertices = startingLineOutsideVertices + nbOutsideVertices + 2;
		const int32 nbInsideVertices = FCString::Atoi(*lines[startingLineInsideVertices - 1]);
		const int32 startingLineTriangles = startingLineInsideVertices + nbInsideVertices + 2;
		const int32 nbTriangles = FCString::Atoi(*lines[startingLineTriangles - 1]);

		// TODO: (Benjamin) Still need to take care of the inside vertices
		TArray<FVector> verticesOut;

		for (int32 i = startingLineOutsideVertices; i < startingLineOutsideVertices + nbOutsideVertices; ++i)
		{
			FString left, right;
			lines[i].Split(TEXT(" "), &left, &right);
			FVector vertex(FCString::Atof(*left), 0.f, FCString::Atof(*right));
			UE_LOG(LogTemp, Warning, TEXT("%s"), *vertex.ToString());
			verticesOut.Add(vertex);
		}
		vertices = verticesOut; // TODO: (Benjamin) Need to be change if we add inside vertices later on
		
		for (int32 i = startingLineTriangles; i < startingLineTriangles + nbTriangles; ++i)
		{
			FString left, right1, right2;
			lines[i].Split(TEXT(" "), &left, &right1);
			triangles.Add(FCString::Atoi(*left));
			right1.Split(TEXT(" "), &left, &right2);
			triangles.Add(FCString::Atoi(*left));
			triangles.Add(FCString::Atoi(*right2));

			int32 j = i - startingLineTriangles;
			UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), triangles[j], triangles[j+1], triangles[j+2]);
		}
	}

	// TODO: (Benjamin) Check for equal vertices, degenerative case?
}

void AProceduralPlatform::Extrude(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals)
{
	const int32 size2DShape = vertices.Num();
	const int32 size3DMesh = size2DShape * 2; // << 1 e_e
	const FVector translationVector = FVector(0.f, -100.f, 0.f);

	// Extruded vertices == translated vertices in y
	for (int i = 0; i < size2DShape; ++i)
		vertices.Add(vertices[i] + translationVector);

	// Triangles creations
	// Global case
	for (int32 i = 1; i < size2DShape; ++i)
	{
		// First triangle
		triangles.Add(i);
		triangles.Add(i - 1);
		triangles.Add(size2DShape + i);
		
		// Second triangle
		triangles.Add(size2DShape + i);
		triangles.Add(i - 1);
		triangles.Add(size2DShape + i - 1);
	}
	// Limit case
	triangles.Add(size2DShape - 1);
	triangles.Add(size3DMesh - 1);
	triangles.Add(0);

	triangles.Add(0);
	triangles.Add(size3DMesh - 1);
	triangles.Add(size2DShape);


	// Normals
	for (int32 i = 0; i < triangles.Num(); i += 3)
	{
		FVector n = FVector::CrossProduct(vertices[triangles[i+2]] - vertices[triangles[i+1]], vertices[triangles[i]] - vertices[triangles[i+1]]);
		normals.Add(n.GetSafeNormal());
	}

}

void AProceduralPlatform::CreatePolygon()
{
	// Read file and define the 2D polygon
	FString fileName("test1");
	TArray<FVector> vertices;
	TArray<int32> triangles;
	GetPolygonFromFile(fileName, vertices, triangles);

	TArray<FVector> normals;
	TArray<FVector2D> uv0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> verticesColors;
	for (int32 i = 0; i < vertices.Num(); ++i)
	{
		normals.Add(FVector(0, 1, 0));
	}

	// Extrude the 2D shape
	Extrude(vertices, triangles, normals);

	// Fill up uv, tangent and color value
	for (int32 i = 0; i < vertices.Num(); ++i)
	{
		uv0.Add(FVector2D(vertices[i].X, vertices[i].Z)); // TODO: (Benjamin) Values of UVs are completely broken so need to be rework if used
		tangents.Add(FProcMeshTangent(0, 0, 1));
		verticesColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	}

	mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uv0, verticesColors, tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void AProceduralPlatform::TriangulatePoly(const TArray<FVector>& vertices, TArray<FVector>& triangles)
{
	using namespace tpp;

	Delaunay::Point tempP;
	std::vector<Delaunay::Point> v;
	for (int i = 0; i < vertices.Num(); ++i)
	{
		tempP[0] = (double)vertices[i].X;
		tempP[1] = (double)vertices[i].Z;
		v.push_back(tempP);
	}

	Delaunay delobject = Delaunay::Delaunay(v);
	delobject.Triangulate();


}