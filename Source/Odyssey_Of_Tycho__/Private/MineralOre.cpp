// Fill out your copyright notice in the Description page of Project Settings.


#include "MineralOre.h"
#include "Tycho_character.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMineralOre::AMineralOre()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OreMesh"));

	/*UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(
		TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;

	OreMesh->SetStaticMesh(CubeMesh);*/
	OreMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	OreMesh->SetupAttachment(RootComponent);
	OreMesh->SetEnableGravity(true);
	OreMesh->GetBodyInstance()->SetMassScale(OreMass);
	Player = GetPlayer();
}


// Called when the game starts or when spawned
void AMineralOre::BeginPlay()
{
	Super::BeginPlay();

	OreMesh->SetSimulatePhysics(true);
}

void AMineralOre::Collecting()
{
	if (!Player)
	{
		Acceleration = 0.f;
		return;
	}
	if (!Player->Get_CanCollect())
	{
		return;
	}
	if (!Player->Get_IsCollecting())
	{
		Acceleration = 0.f;
		return;
	}


	PlayerDistance = FVector::Distance(this->GetActorLocation(), Player->GetActorLocation());
	if (PlayerDistance > Player->Get_CollectingDistanceMax())
	{
		return;
	}
	const float X = this->GetActorLocation().X;
	const float Y = this->GetActorLocation().Y;
	const float Z = this->GetActorLocation().Z;

	const float X_Player = Player->GetActorLocation().X;
	const float Y_Player = Player->GetActorLocation().Y;
	const float Z_Player = Player->GetActorLocation().Z;
	if (PlayerDistance <= Player->Get_CollectingDistanceMin())
	{
		if (Player->Add_Minerals(this->StaticClass(), Cost))
		{
			Destroy();
		}
	}

	/// Impulce -> to player -> acceleration
	if (Player->CosinePlayerToObject(GetActorLocation()) < 0.6)
	{
		return;
	}
	if (PlayerDistance >= Player->Get_CollectingDistanceMin())
		AddForce(ImpulseParabola(PlayerDistance), FVector(X_Player - X, Y_Player - Y, Z_Player - Z).GetSafeNormal());
}

float AMineralOre::ImpulseParabola(float PlayerDistance_) const
{
	const float m = 0;
	const float n = Player->Get_CollectingForce() / 5;

	const float p = Player->Get_CollectingDistanceMax();
	const float r = 0;

	const float w = Player->Get_CollectingForce();
	const float s = p / 2;
	const float a = (n * p + m * w + r * s - w * p - s * n - r * m) / (FMath::Pow(m, 2) * p + FMath::Pow(s, 2) * m + s *
		FMath::Pow(p, 2) - FMath::Pow(s, 2) * p - s * FMath::Pow(m, 2) - FMath::Pow(p, 2) * m);

	const float b = (FMath::Pow(m, 2) * r + n * FMath::Pow(s, 2) + FMath::Pow(p, 2) * w - FMath::Pow(s, 2) * r -
		FMath::Pow(m, 2) * w - FMath::Pow(p, 2) * n) / (FMath::Pow(m, 2) * p + FMath::Pow(s, 2) * m + s *
		FMath::Pow(p, 2) - FMath::Pow(s, 2) * p - s * FMath::Pow(m, 2) - FMath::Pow(p, 2) * m);

	const float c = n;

	return a * (PlayerDistance_) * (PlayerDistance_) + b * (PlayerDistance_) + c;
}

// Called every frame
void AMineralOre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Player = GetPlayer();


	Collecting();
}

void AMineralOre::AddForce(const float Acceleration_Set, const FVector& Direction)
{
	Acceleration = Acceleration_Set;
	AccelerationDirection = Direction;
	OreMesh->AddImpulse(AccelerationDirection * Acceleration * OreMass / 10);
}

void AMineralOre::Set_Cost(const float Cost_Set)
{
	Cost = Cost_Set;
}

ATycho_Character* AMineralOre::GetPlayer() const
{
	if (GetWorld())
	{
		static ATycho_Character* Mallet = nullptr;
		if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			Mallet = Cast<ATycho_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		}


		if (!Mallet)
		{
			return nullptr;
		}
		return Mallet;
	}
	return nullptr;
}
