// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "MineralObject.generated.h"

class AMineralOre;
class ATycho_Character;

UCLASS()
class ODYSSEY_OF_TYCHO___API AMineralObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMineralObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostLoad() override;

	static float Get_OptimalCosine(float Player_DrillDistance, float Player_Distance);
	void TakeDrillDamage();
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void OreSpawner(const FVector& Location) const;

	long long Object_ID;

	float MineralMass = 5000.f;
	inline static long long Object_Counter = -1;
	float Endurance = 100.f; /// ----- decrease by drilling
	int MineralsAmount = 10; /// ----- collect after drilling
	float MineralCost = 20; /// ----- every mineral cost
	float Timer = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* MineralMesh;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"),Category = "Ore Class")
	TSubclassOf<AMineralOre> OreClass;
	
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	ATycho_Character* Player;
	float PlayerDistance;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ATycho_Character* GetPlayer() const;
};
