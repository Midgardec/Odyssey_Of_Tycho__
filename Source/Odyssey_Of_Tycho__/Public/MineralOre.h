// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MineralOre.generated.h"

class ATycho_Character;

UCLASS()
class ODYSSEY_OF_TYCHO___API AMineralOre : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMineralOre();

	void AddForce(float Acceleration, const FVector& Direction);
	void Set_Cost(float Cost);
	ATycho_Character* GetPlayer() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Collecting();
	float ImpulseParabola(float PlayerDistance_) const;
	float Cost;
	float OreMass = 100.f;
	float Velocity = 0;
	float Acceleration = 0;
	FVector AccelerationDirection = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* OreMesh;
	

private:
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	ATycho_Character* Player = nullptr;
	float PlayerDistance;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
