// Fill out your copyright notice in the Description page of Project Settings.


#include "PetBot.h"
#include "Tycho_character.h"

// Sets default values
APetBot::APetBot()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIController = CreateDefaultSubobject<AAIController>(TEXT("Controller"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetMesh()->SetMassScale(NAME_None,50.f);
}

// Called when the game starts or when spawned
void APetBot::BeginPlay()
{
	Super::BeginPlay();
}

void APetBot::FollowPlayer()
{
	if (!Player) { return; }
	if (!AIController) { return; }
	GetCharacterMovement()->MaxWalkSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
	float distanceToPlayer = FMath::Sqrt(
		FMath::Square(this->GetActorLocation().X - Player->GetActorLocation().X) +
		FMath::Square(this->GetActorLocation().Y - Player->GetActorLocation().Y));
	//FVector::Distance(this->GetActorLocation(), Player->GetActorLocation());
	if (distanceToPlayer >= AcceptableDistanceToPlayer)
	{
		FVector DirectionVector = FVector(Player->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		AddMovementInput(DirectionVector, 1);
	}
	else if (distanceToPlayer <= 200.f)
	{
		FVector DirectionVector = -FVector(Player->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		AddMovementInput(DirectionVector, 1);
	}

	//AIController->MoveToActor(Player, 150.f);
}

void APetBot::FollowTarget()
{
	if (!Target) { return; }
	if (!AIController) { return; }
	float distanceToTarget = FVector::Distance(this->GetActorLocation(), Target->GetActorLocation());
	if (distanceToTarget >= AcceptableDistanceToTarget)
	{
		FVector DirectionVector = FVector(Target->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		AddMovementInput(DirectionVector, FollowTargetSpeed);
	}
	//AIController->MoveToActor(Target, 150.f);
}

ATycho_Character* APetBot::Get_Player() const
{
	if (!Player)
	{
		return nullptr;
	}
	return Player;
}

void APetBot::Set_Player(ATycho_Character* Player_)
{
	this->Player = Player_;
}

AActor* APetBot::Get_Target() const
{
	if (!Target)
	{
		return nullptr;
	}
	return Target;
}

void APetBot::Set_Target(AActor* Target_)
{
	this->Target = Target_;
}

// Called every frame
void APetBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Target)
	{
		FollowTarget();
	}
	else
	{
		FollowPlayer();
	}
	//FollowPlayer();
}

// Called to bind functionality to input
void APetBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
