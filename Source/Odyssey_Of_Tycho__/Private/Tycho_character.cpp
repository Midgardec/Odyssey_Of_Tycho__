// Fill out your copyright notice in the Description page of Project Settings.


#include "Tycho_character.h"

#include "MineralObject.h"

#include "Kismet/GameplayStatics.h"

#include "TestManager.h"
#include "TSEventManager.h"
#include "TravelManager.h"
// Sets default values
ATycho_Character::ATycho_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	///
	/// backpack
	//Backpack_Size = 800.f;
	Backpack_SpaceLeft = Backpack_Size;


}

// Called when the game starts or when spawned
void ATycho_Character::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Pets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APetBot::StaticClass(), Pets);
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1,15.f, FColor::Red, FString::Printf(TEXT("pets number : %i"),Pets.Num()));
	}
	for (auto Pet : Pets)
	{
		if (APetBot* PetBot = Cast<APetBot>(Pet))
		{
			if (!PetBot->Get_Player())
			{
				PetBot->Set_Player(this);
				PetFollower = PetBot;
				break;
			}
		}
	}
	/// TestManager
	TArray<AActor*> TestManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestManager::StaticClass(), TestManagers);

	for (auto TM : TestManagers)
	{
		m_TestManager = Cast<ATestManager>(TM);
	}
	if (m_TestManager) {
		m_TestManager->Set_Player(this);
	}

	/// EventManager
	TArray<AActor*> EventManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATSEventManager::StaticClass(), EventManagers);

	for (auto EM : EventManagers)
	{
		m_EventManager = Cast<ATSEventManager>(EM);
	}
}


// Called every frame
void ATycho_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanDrill) {
		DrillTimer += DeltaTime;
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Drill timer: %f"), DrillTimer));
	}
	if (IsDrilling && CanDrill) {
		DrillMineralObject();
	}
	if (DrillTimer >= 0.5f) {
		CanDrill = true;
		DrillTimer = 0.f;
	}

	/*if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn() == this)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}*/
}

// Called to bind functionality to input
void ATycho_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATycho_Character::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATycho_Character::EndCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATycho_Character::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATycho_Character::EndSprint);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATycho_Character::Interact);
	PlayerInputComponent->BindAction("EnterExitSpaceship", IE_Pressed, this, &ATycho_Character::EnterSpaceShip);

	PlayerInputComponent->BindAction("Drilling", IE_Pressed, this, &ATycho_Character::Drill);
	PlayerInputComponent->BindAction("Drilling", IE_Released, this, &ATycho_Character::StopDrill);

	PlayerInputComponent->BindAction("Collecting", IE_Pressed, this, &ATycho_Character::Collect);
	PlayerInputComponent->BindAction("Collecting", IE_Released, this, &ATycho_Character::StopCollect);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATycho_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATycho_Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ATycho_Character::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ATycho_Character::LookUp);

	PlayerInputComponent->BindAction("TargetPointer", IE_Pressed, this, &ATycho_Character::SetTarget);

	PlayerInputComponent->BindAction("key1", IE_Pressed, this, &ATycho_Character::KeyPressedOne);
	PlayerInputComponent->BindAction("key2", IE_Pressed, this, &ATycho_Character::KeyPressedTwo);
	PlayerInputComponent->BindAction("key3", IE_Pressed, this, &ATycho_Character::KeyPressedThree);

}

///
/// -------------------------------------  movement 
void ATycho_Character::MoveForward(const float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATycho_Character::MoveRight(const float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATycho_Character::Turn(const float Value)
{
	AddControllerYawInput(Value);
}

void ATycho_Character::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

void ATycho_Character::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void ATycho_Character::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ATycho_Character::BeginCrouch()
{
	Crouch();
}

void ATycho_Character::EndCrouch()
{
	UnCrouch();
}

///
/// -------------------------------------  interaction, drilling, collecting, etc.
void ATycho_Character::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Interacting"));
}

void ATycho_Character::EnterSpaceShip()
{
	TArray<AActor*> Spaceships;
	GetOverlappingActors(Spaceships, TSubclassOf<ASpaceship>());
	for (auto Spaceship : Spaceships)
	{
		if (Spaceship)
		{
			ASpaceship* Possessed_Spaceship = StaticCast<ASpaceship*>(Spaceship);
			Possessed_Spaceship->Set_Player(this);
			this->SetActorHiddenInGame(true);
			this->SetActorEnableCollision(false);
			//this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if (m_EventManager) {
				m_EventManager->ProcessEventType(EEventType::EventSpaceship);
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(Possessed_Spaceship);
			}
			Possessed_Spaceship->ProcessPossession();
			
			return;
		}
	}
}

void ATycho_Character::Set_IsPossessed(bool State)
{
	IsPossessed = State;
}

bool ATycho_Character::Get_IsPossessed() const
{
	return IsPossessed;
}

void ATycho_Character::Drill()
{
	if (CanDrill)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Drilling"));

		IsDrilling = true;
	}
}
void ATycho_Character::DrillMineralObject() {
	if (AActor* Obj = FindObjectToDrill()) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Drilled")));
		FHitResult HitOut;
		UGameplayStatics::ApplyPointDamage(Obj, DrillingStrength, FVector(), HitOut, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
		CanDrill = false;
	}
}
void ATycho_Character::StopDrill()
{
	IsDrilling = false;
}

AActor* ATycho_Character::FindObjectToDrill()
{
	TArray<AActor*> MineralObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMineralObject::StaticClass(), MineralObjects);
	AActor* ObjToDrill = nullptr;
	float ObjMinDistance = DrillDistance + 0.5f;
	for (auto MineralObj : MineralObjects)
	{
		float DistanceToObj = FVector::Distance(this->GetActorLocation(), MineralObj->GetActorLocation());

		if (DistanceToObj * CosinePlayerToObject(MineralObj->GetActorLocation()) < ObjMinDistance
			&& CosinePlayerToObject(MineralObj->GetActorLocation()) > Get_OptimalCosine(DrillDistance, DistanceToObj))
		{
			ObjMinDistance = DistanceToObj * CosinePlayerToObject(MineralObj->GetActorLocation());
			ObjToDrill = MineralObj;
		}
	}
	/// DONE
	/// TODO: find mineralObject[min(distance * cosineToForwVect)]
	return ObjToDrill;
}

float ATycho_Character::Get_OptimalCosine(const float Player_DrillDistance, const float Player_Distance)
{
	const float X1 = Player_DrillDistance;
	constexpr float X2 = 0;
	constexpr float Y1 = 1; // max cosine
	constexpr float Y2 = 0.1; // min cosine

	return (Player_Distance - X2) / (X1 - X2) * (Y1 - Y2) + Y2;
}

float ATycho_Character::CosinePlayerToObject(const FVector& ObjectLocation) const
{
	const FVector ForwardVectorNormal = GetActorForwardVector().GetSafeNormal();
	const float x = ObjectLocation.X - GetActorLocation().X;
	const float y = ObjectLocation.Y - GetActorLocation().Y;
	const float z = ObjectLocation.Z - GetActorLocation().Z;
	const FVector PlayerToObjectVectorNormal = FVector(x, y, z).GetSafeNormal();
	const float CosinePO = ForwardVectorNormal.Dot(PlayerToObjectVectorNormal) / PlayerToObjectVectorNormal.Length();

	return CosinePO;
}

void ATycho_Character::KeyPressedOne()
{
	FString outMessage;
	if (m_TestManager) {
		m_TestManager->CheckRightAnswer(0, outMessage);

	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Pressed 1")));
}

void ATycho_Character::KeyPressedTwo()
{
	FString outMessage;
	if (m_TestManager) {
		m_TestManager->CheckRightAnswer(1, outMessage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Pressed 2")));
}

void ATycho_Character::KeyPressedThree()
{
	FString outMessage;
	if (m_TestManager) {
		m_TestManager->CheckRightAnswer(2, outMessage);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Pressed 3")));
}

void ATycho_Character::AddCoins(int Amount)
{
	m_Coins += Amount;
	m_Coins = FMath::Max(0, m_Coins);
}

void ATycho_Character::GetCoinsAmount(int& outCoins)
{
	outCoins = m_Coins;
}

void ATycho_Character::Collect()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("Collecting"));
	IsCollecting = true;
}

void ATycho_Character::StopCollect()
{
	IsCollecting = false;
}

long long ATycho_Character::Get_CurrentDrillObject_ID() const
{
	return CurrentDrillObject_ID;
}


void ATycho_Character::Set_CurrentDrillObject_ID(const long long Object_ID)
{
	if (CurrentDrillObject_ID != -1 && Object_ID != -1)
	{
		return;
	}
	CurrentDrillObject_ID = Object_ID;
}

float ATycho_Character::Get_CurrentDrillObject_Distance() const
{
	return CurrentDrillObject_Distance;
}

void ATycho_Character::Set_CurrentDrillObject_Distance(const float CurrentDrillObject_Distance_ref)
{
	this->CurrentDrillObject_Distance = CurrentDrillObject_Distance_ref;
}

bool ATycho_Character::Get_CanDrill() const
{
	return CanDrill;
}

void ATycho_Character::Set_CanDrill(bool State)
{
	CanDrill = State;
}

void ATycho_Character::Set_IsDrilling(bool State)
{
	IsDrilling = State;
}

bool ATycho_Character::Get_IsCollecting() const
{
	return IsCollecting;
}

bool ATycho_Character::Get_IsDrilling() const
{
	return IsDrilling;
}

float ATycho_Character::Get_DrillingStrength() const
{
	return DrillingStrength;
}

float ATycho_Character::Get_DrillDistance() const
{
	return DrillDistance;
}

void ATycho_Character::Set_IsCollecting(bool State)
{
	IsCollecting = State;
}

void ATycho_Character::Set_CanCollect(bool State)
{
	CanCollect = State;
}

float ATycho_Character::Get_CollectingDistanceMax() const
{
	return CollectingDistanceMax;
}

float ATycho_Character::Get_CollectingDistanceMin() const
{
	return CollectingDistanceMin;
}

float ATycho_Character::Get_CollectingForce() const
{
	return CollectingForce;
}

bool ATycho_Character::Get_CanCollect() const
{
	return CanCollect;
}

bool ATycho_Character::Add_Minerals(UClass* ClassType, float Amount)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
									 FString::Printf(
										 TEXT("Materials Amount %i"), static_cast<int>(CollectedMaterials_ClassA)));
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,
									 FString::Printf(
										 TEXT("Backpack Space left a %i"),
										 static_cast<int>(Backpack_SpaceLeft - Amount)));*/

	if (static_cast<int>(Backpack_SpaceLeft - Amount) < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			FString::Printf(TEXT("Can put in? %i"), static_cast<int>(false)));
		Set_CanCollect(false);
		return false;
	}
	/// по хорошему в класс добавить массив и по индексу-ClassType добавлять Amount;
	CollectedMaterials_ClassA += Amount;
	Backpack_SpaceLeft -= Amount;
	Set_CanCollect(true);
	return true;
}



/// ---- pet

void ATycho_Character::SetTarget()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Camera->GetForwardVector() * 10000 + Start;
	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5, 0, 5);
	bool isHit = ActorLineTraceSingle(OutHit, Start, End, ECC_WorldStatic, QueryParams);
	if (isHit)
	{
		//->AddOnScreenDebugMessage(-1,15.f, FColor::Cyan, FString::Printf(TEXT("hit detected")));

		DrawDebugSphere(GetWorld(), FVector(OutHit.Location), 3.f, 3, FColor::Red);
		if (OutHit.GetActor())
		{
			PetFollower->Set_Target(Cast<AActor>(OutHit.GetActor()));
		}


	}
}

UCameraComponent* ATycho_Character::GetCamera()
{
	return Camera;
}
