// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Spaceship.h"
#include "PetBot.h"

#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Tycho_character.generated.h"

class ATestManager;
class ATSEventManager;

UCLASS(config = Game)
class ODYSSEY_OF_TYCHO___API ATycho_Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	// Sets default values for this character's properties
	ATycho_Character();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Interact();

	void EnterSpaceShip();

	void Drill();
	void DrillMineralObject();
	void StopDrill();
	AActor* FindObjectToDrill();

	float Get_OptimalCosine(const float Player_DrillDistance, const float Player_Distance);

	void Collect();
	void StopCollect();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);
	FVector2D CameraInput;

	//Sets Character Movement Speed to Sprint values.
	void BeginSprint();
	void EndSprint();

	//Request for Character to Crouch
	void BeginCrouch();
	void EndCrouch();

	// Set_CanCollect
	void Set_CanCollect(bool State);

private:
	float Health;
	float Cash;
	float OxygenCapacity;

	float DrillingStrength = 20.f;
	float DrillDistance = 500.f;
	float DrillTimer = 0.f;
	bool CanDrill = true;
	bool IsDrilling = false;

	float CollectingDistanceMax = 1000.f;
	float CollectingDistanceMin = 100.f;
	float CollectingForce = 100.f;
	float CollectedMaterials_ClassA = 0;
	bool IsCollecting = false;
	bool CanCollect = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "BackPack|Size")
	float Backpack_Size = 1500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "BackPack|FreeSpace")
	float Backpack_SpaceLeft;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	long long CurrentDrillObject_ID = -1;

	/// is or is not in spaseship
	bool IsPossessed;

	float CurrentDrillObject_Distance = -1;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	APetBot* PetFollower;
	void SetTarget();


public:
	UCameraComponent* GetCamera();
	long long Get_CurrentDrillObject_ID() const;
	void Set_CurrentDrillObject_ID(long long Object_ID);
	float Get_CurrentDrillObject_Distance() const;
	void Set_CurrentDrillObject_Distance(const float CurrentDrillObject_Distance_ref);
	/// --- possessness
	void Set_IsPossessed(bool);
	bool Get_IsPossessed() const;
	/// --- drill
	void Set_CanDrill(bool State);
	void Set_IsDrilling(bool);

	bool Get_CanDrill() const;
	bool Get_IsDrilling() const;
	float Get_DrillingStrength() const;
	float Get_DrillDistance() const;

	/// --- collect
	void Set_IsCollecting(bool);

	bool Get_IsCollecting() const;
	float Get_CollectingDistanceMax() const;
	float Get_CollectingDistanceMin() const;
	float Get_CollectingForce() const;
	bool Get_CanCollect() const;

	///
	bool Add_Minerals(UClass* ClassType, float Amount);

	///
	float CosinePlayerToObject(const FVector& ObjectLocation) const;



	/// --- TestSystem
	void KeyPressedOne();
	void KeyPressedTwo();
	void KeyPressedThree();

	/// --- TestSystemBonuses;
	void AddCoins(int Amount);
	UFUNCTION(BlueprintCallable)
	void GetCoinsAmount(int& outCoins);
private:
	/// TestManager
	TObjectPtr<ATestManager> m_TestManager;
	/// EventManager
	TObjectPtr<ATSEventManager> m_EventManager;

	/// Coins
	int m_Coins = 0;
public:
	/// --------------------------------------- UE defaults

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
