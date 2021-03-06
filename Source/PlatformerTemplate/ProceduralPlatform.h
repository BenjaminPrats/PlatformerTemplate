// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ScreenMessage.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralPlatform.generated.h"

UCLASS()
class PLATFORMERTEMPLATE_API AProceduralPlatform : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	void GetPolygonFromFile(const FString& fileName, TArray<FVector>& vertices, TArray<int32>& polygons);
	void Extrude(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

public:	
	// Sets default values for this actor's properties
	AProceduralPlatform();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void CreateTriangle();
	void CreatePolygon();
};
