// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Spaceship.generated.h"

class ATycho_Character;
class ATestManager;
class ATSEventManager;
UCLASS()
class ODYSSEY_OF_TYCHO___API ASpaceship : public APawn
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** Movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* MovementComponent;

	/** Mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	/** Exit Location component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ExitLocation;

protected:
	bool isInSpace = false;
	
	float MaxThrust = 5000.f;
	float MaxVerticalThrust = 5000.f;
	float MaxHorizontalThrust = 5000.f;
	float Thrust;
	float VerticalThrust;
	float HorizontalThrust;
	FVector ForwardVelocity;
	FVector UpVelocity;
	FVector RightVelocity;

	bool LandingMode = true;
	double PreLandingMaxVelocity = 1000.f;
	
	float RotationRate = 50.f;

	float minMouse=100;
	float maxMouse=-100;
public:
	// Sets default values for this pawn's properties
	ASpaceship();

	void Set_Player(ATycho_Character* Player_);
	UFUNCTION(BlueprintCallable)
	ATycho_Character* Get_Player() const;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DesiredYaw = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DesiredPitch = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AsinPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AsinYaw ;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void ExitSpaceShip();
	UPROPERTY()
	TObjectPtr<ATycho_Character> Player = nullptr;

	void BeginSpeedShift();
	void EndSpeedShift();
	/// spaceship
	void Spaceship_Roll(float Value) ;
	void Spaceship_Vertical(float Y);
	void MoveRight(float Value);
	void MoveForward(float W);
	/// mouse
	void Turn(float Value);
	void LookUp(float Value);

public:
	void RotationStabilization();
	void ApplyVelocity();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// --- TestSystem
	void KeyPressedOne();
	void KeyPressedTwo();
	void KeyPressedThree();
private:
	/// TestManager
	ATestManager* m_TestManager;
	/// EventtManager
	ATSEventManager* m_EventManager;

public:
	/// ------
	void LandingModeToggle();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void ProcessPossession();
};
