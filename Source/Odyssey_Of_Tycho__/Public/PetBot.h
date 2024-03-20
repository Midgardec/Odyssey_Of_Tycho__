// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PetBot.generated.h"


class ATycho_Character;

UCLASS()
class ODYSSEY_OF_TYCHO___API APetBot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APetBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FollowPlayer();
	void FollowTarget();

public:
	ATycho_Character* Get_Player() const;
	void Set_Player(ATycho_Character* Player);
	AActor* Get_Target() const;
	void Set_Target(AActor* Target);

protected:
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	AAIController* AIController;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	AActor* Target = nullptr;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	ATycho_Character* Player = nullptr;
	
	UPROPERTY(EditAnywhere)
	float AcceptableDistanceToPlayer = 350.f;
	UPROPERTY(EditAnywhere)
	float FollowPlayerSpeed = 300.f;
	UPROPERTY(EditAnywhere)
	float AcceptableDistanceToTarget;
	UPROPERTY(EditAnywhere)
	float FollowTargetSpeed = 300.f;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
