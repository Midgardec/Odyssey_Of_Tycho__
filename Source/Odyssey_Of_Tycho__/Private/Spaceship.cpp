// Fill out your copyright notice in the Description page of Project Settings.


#include "Spaceship.h"


#include "VectorTypes.h"
#include "Tycho_character.h"

#include "TestManager.h"
#include "TSEventManager.h"
#include "TravelManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpaceship::ASpaceship()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	MeshComponent->SetupAttachment(RootComponent);
	// Configure movement

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(MeshComponent);
	SpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	//MovementComponent->bOrientRotationToMovement = true;
	/// physics
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetEnableGravity(false);


	ExitLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ExitLocationComponent"));
	ExitLocation->SetupAttachment(MeshComponent);
}

void ASpaceship::Set_Player(ATycho_Character* Player_)
{
	Player = Player_;
}

ATycho_Character* ASpaceship::Get_Player() const
{
	return Player;
}

// Called when the game starts or when spawned
void ASpaceship::BeginPlay()
{
	Super::BeginPlay();

	/// TestManager
	TArray<AActor*> TestManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestManager::StaticClass(), TestManagers);

	for (auto TM : TestManagers)
	{
		m_TestManager = Cast<ATestManager>(TM);
	}
	/// EventManager
	TArray<AActor*> EventManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATSEventManager::StaticClass(), EventManagers);

	for (auto EM : EventManagers)
	{
		m_EventManager = Cast<ATSEventManager>(EM);
	}
}

void ASpaceship::ExitSpaceShip()
{
	if (!Player)
	{
		return;
	}
	TArray<AActor*> TravelManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATravelManager::StaticClass(), TravelManagers);
	for (auto tm : TravelManagers) {
		if (tm) {
			ATravelManager* level_travelManager = StaticCast<ATravelManager*>(tm);
			FString MES_SET_SPCSHP;
			level_travelManager->SetSpaceship(nullptr, MES_SET_SPCSHP);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, MES_SET_SPCSHP);
			break;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(L"all bad"));
		}
	}
	Player->SetActorHiddenInGame(false);
	Player->SetActorEnableCollision(true);
	Player->SetActorLocation(ExitLocation->GetComponentLocation());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(Player);
}

void ASpaceship::BeginSpeedShift()
{
	MaxThrust = 50000.f;
}

void ASpaceship::EndSpeedShift()
{
	MaxThrust = 5000.f;
}

void ASpaceship::Spaceship_Roll(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const float New_X = 60 * Value;
		const FVector Roll_Torque = GetActorForwardVector() * New_X;
		const FVector Roll = FMath::Lerp(FVector(0, 0, 0), GetActorForwardVector() * New_X, 0.1);
		MeshComponent->AddTorqueInRadians(Roll, TEXT("None"), true);
	}
}


void ASpaceship::Spaceship_Vertical(float Value)
{
	VerticalThrust = FMath::FInterpTo(VerticalThrust,
		FMath::Lerp(MaxVerticalThrust / 2, MaxVerticalThrust, (Value + 1) / 2),
		GetWorld()->GetDeltaSeconds(),
		0.5);
	if ((Controller != nullptr) && (Value > 0.0f))
	{
		UpVelocity = GetActorUpVector() * VerticalThrust;
	}
	else if ((Controller != nullptr) && (Value < 0.0f))
	{
		UpVelocity = GetActorUpVector() * -VerticalThrust;
	}
	else
	{
		UpVelocity = GetActorForwardVector() * 0;
	}
}

void ASpaceship::MoveRight(float Value)
{
	HorizontalThrust = FMath::FInterpTo(HorizontalThrust, FMath::Lerp(0, MaxHorizontalThrust, (Value + 1) / 2),
		GetWorld()->GetDeltaSeconds(),
		0.5);
	/// Calculating forward thrust
	if ((Controller != nullptr) && (Value > 0.0f))
	{
		//const float New_W = W * MaxThrust;

		RightVelocity = GetActorRightVector() * HorizontalThrust;
		//MeshComponent->SetPhysicsLinearVelocity(Thrust);
	}
	else if ((Controller != nullptr) && (Value < 0.0f))
	{
		RightVelocity = GetActorRightVector() * -HorizontalThrust;
	}
	else
	{
		RightVelocity -= RightVelocity * 1;
	}
}

void ASpaceship::MoveForward(float Value)
{
	Thrust = FMath::FInterpTo(Thrust, FMath::Lerp(0, MaxThrust, (Value + 1) / 2),
		GetWorld()->GetDeltaSeconds(),
		0.5);
	/// Calculating forward thrust
	if ((Controller != nullptr) && (Value > 0.0f))
	{
		ForwardVelocity = GetActorForwardVector() * Thrust;
	}
	else if ((Controller != nullptr) && (Value < 0.0f))
	{
		ForwardVelocity = GetActorForwardVector() * -Thrust;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, FString::Printf(TEXT("not moving forward")));
		if (!isInSpace)
		{
			ForwardVelocity -= ForwardVelocity * 1;
		}

	}
}

void ASpaceship::Turn(float Value)
{
	DesiredYaw += Value * RotationRate * 0.005;
	DesiredYaw = FMath::Clamp(DesiredYaw, -90, 90);

	/*DesiredYaw = FMath::Lerp(0, RotationRate, Value);*/
	if (FMath::Abs(Value) < 0.01)
	{
		DesiredYaw -= DesiredYaw * 0.1;
	}
	/*AddControllerYawInput(Value);*/
}

void ASpaceship::LookUp(float Value)
{
	//DesiredPitch = -FMath::Lerp(0, RotationRate, Value);
	DesiredPitch += -Value * RotationRate * 0.005;
	DesiredPitch = FMath::Clamp(DesiredPitch, -90, 90);
	if (FMath::Abs(Value) < 0.01)
	{
		DesiredPitch -= DesiredPitch * 0.1;
	}

	/*AddControllerPitchInput(Value);*/
}

void ASpaceship::RotationStabilization()
{
	const float FVOptLen = RotationRate / 2;
	const float SinPitch = DesiredPitch / sqrt(
		pow(GetActorForwardVector().Length() * FVOptLen, 2) + pow(DesiredPitch, 2));
	const float SinYaw = DesiredYaw / sqrt(pow(GetActorForwardVector().Length() * FVOptLen, 2) + pow(DesiredYaw, 2));
	AsinPitch = FMath::Asin(SinPitch);
	AsinYaw = FMath::Asin(SinYaw);
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta,FString::Printf(TEXT("Angle up/ right: %f === %f"), AsinPitch, AsinYaw));
	FRotator NewRotation = FRotator(AsinPitch, AsinYaw, 0);
	/*FMath::RInterpTo(this->GetActorRotation(),this->GetActorRotation() + FRotator(DesiredPitch, DesiredYaw, 0), DeltaTime,10);*/
	if (FMath::Abs(AsinPitch) > 0.5)
	{
		MeshComponent->AddTorqueInDegrees(-AsinPitch * GetActorRightVector() * RotationRate, TEXT("None"), true);
	}
	if (FMath::Abs(AsinYaw) > 0.5)
	{
		MeshComponent->AddTorqueInDegrees(AsinYaw * GetActorUpVector() * RotationRate, TEXT("None"), true);
	}
	MeshComponent->AddTorqueInDegrees((-MeshComponent->GetPhysicsAngularVelocityInDegrees()), TEXT("None"), true);
}

void ASpaceship::ApplyVelocity()
{
	MeshComponent->AddImpulse(ForwardVelocity + UpVelocity + RightVelocity);
}

// Called every frame
void ASpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta,FString::Printf(TEXT("Velocity: %f"), MeshComponent->GetComponentVelocity().Length()));

	ApplyVelocity();

	/// Rotation stabilization;
	//MeshComponent->AddTorqueInDegrees((-MeshComponent->GetPhysicsAngularVelocityInDegrees()), TEXT("None"), true);
	RotationStabilization();



	//MeshComponent->SetWorldRotation(FMath::RInterpTo(MeshComponent->GetRelativeRotation(), MeshComponent->GetRelativeRotation() + newRotation, DeltaTime,5));


	/*DesiredPitch = newRotation.Pitch - MeshComponent->GetRelativeRotation().Pitch;
	DesiredYaw = newRotation.Yaw - MeshComponent->GetRelativeRotation().Yaw;*/
}

void ASpaceship::KeyPressedOne()
{
	FString outMessage;
	if (m_TestManager) {
		m_TestManager->CheckRightAnswer(0, outMessage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Pressed 1")));
}
void ASpaceship::KeyPressedTwo()
{
	FString outMessage;
	if (m_TestManager) {
		m_TestManager->CheckRightAnswer(1, outMessage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Pressed 2")));
}
void ASpaceship::KeyPressedThree()
{
	FString outMessage;
	if (m_TestManager) {
		m_TestManager->CheckRightAnswer(2, outMessage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Pressed 3")));
}

void ASpaceship::LandingModeToggle()
{
	if (MeshComponent->GetComponentVelocity().Length() <= PreLandingMaxVelocity)
	{
		//MeshComponent->AddImpulse(-MeshComponent->GetComponentVelocity());

		LandingMode = !LandingMode;
		/*
		/// TODO:
		///	MeshComponent->SetEnableGravity(LandingMode);
		/// maybe disable inputs and AddImpulse downward
		///
		/// ( calculate down direction : maybe add sphere collision and rotate to landing zone =>
		/// => need to define landing zone =>
		/// => if statement need to be redesigned with new landing zone component).
		*/
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Turquoise,FString::Printf(TEXT("Is In LandingMod: %i"), LandingMode));
	}
}

// Called to bind functionality to input
void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAction("EnterExitSpaceship", IE_Pressed, this, &ASpaceship::ExitSpaceShip);

	PlayerInputComponent->BindAction("LandingModeToggle", IE_Pressed, this, &ASpaceship::LandingModeToggle);


	PlayerInputComponent->BindAction("Spaceship_SpeedShift", IE_Pressed, this, &ASpaceship::BeginSpeedShift);
	PlayerInputComponent->BindAction("Spaceship_SpeedShift", IE_Released, this, &ASpaceship::EndSpeedShift);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpaceship::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceship::MoveRight);
	PlayerInputComponent->BindAxis("Spaceship_Roll", this, &ASpaceship::Spaceship_Roll);
	PlayerInputComponent->BindAxis("Spaceship_Vertical", this, &ASpaceship::Spaceship_Vertical);

	PlayerInputComponent->BindAxis("Turn", this, &ASpaceship::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASpaceship::LookUp);


	PlayerInputComponent->BindAction("key1", IE_Pressed, this, &ASpaceship::KeyPressedOne);
	PlayerInputComponent->BindAction("key2", IE_Pressed, this, &ASpaceship::KeyPressedTwo);
	PlayerInputComponent->BindAction("key3", IE_Pressed, this, &ASpaceship::KeyPressedThree);

}

void ASpaceship::ProcessPossession()
{
	TArray<AActor*> TravelManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATravelManager::StaticClass(), TravelManagers);
	for (auto tm : TravelManagers) {
		if (tm) {
			ATravelManager* level_travelManager = StaticCast<ATravelManager*>(tm);
			FString MES_SET_SPCSHP;
			level_travelManager->SetSpaceship(this, MES_SET_SPCSHP);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, MES_SET_SPCSHP);
			break;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(L"all bad"));
		}
	}

}
