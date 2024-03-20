// Fill out your copyright notice in the Description page of Project Settings.


#include "MineralObject.h"

#include "MineralOre.h"
#include "Tycho_character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMineralObject::AMineralObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Player = GetPlayer();

	Object_Counter++;
	Object_ID = Object_Counter;
	if (GEngine)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,
		                                 FString::Printf(TEXT("object id = %llu"), Object_ID));*/
	}

	MineralMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineralMesh"));
	MineralMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	MineralMesh->SetupAttachment(RootComponent);
	MineralMesh->SetEnableGravity(true);
	MineralMesh->GetBodyInstance()->SetMassScale(MineralMass);
}


void AMineralObject::BeginPlay()
{
	Super::BeginPlay();
	MineralMesh->SetSimulatePhysics(true);
	Object_Counter = 0;
}

void AMineralObject::PostLoad()
{
	Super::PostLoad();
}

float AMineralObject::Get_OptimalCosine(const float Player_DrillDistance, const float Player_Distance)
{
	const float X1 = Player_DrillDistance;
	constexpr float X2 = 0;
	constexpr float Y1 = 1; // max cosine
	constexpr float Y2 = 0.1; // min cosine
	
	return (Player_Distance - X2) / (X1 - X2) * (Y1 - Y2) + Y2;
}

void AMineralObject::TakeDrillDamage()
{
	/*if (!Player)
	{
		MineralMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
		return;
	}
	PlayerDistance = FVector::Distance(Player->GetActorLocation(), this->GetActorLocation());

	if (Player->Get_CurrentDrillObject_ID() == Object_ID)
	{
		if (Player->CosinePlayerToObject(GetActorLocation()) < Get_OptimalCosine(
			Player->Get_DrillDistance(), PlayerDistance))
		{
			Player->Set_CurrentDrillObject_ID(-1);
			Player->Set_CurrentDrillObject_Distance(-1);
			MineralMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
			return;
		}
	}
	if (PlayerDistance >= Player->Get_DrillDistance())
	{
		if (Player->Get_CurrentDrillObject_ID() == Object_ID)
		{
			Player->Set_CurrentDrillObject_ID(-1);
			Player->Set_CurrentDrillObject_Distance(-1);
			MineralMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
		}

		return;
	}
	if (!Player->Get_IsDrilling())
	{
		MineralMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
		return;
	}
	

	if (Player->CosinePlayerToObject(GetActorLocation()) < Get_OptimalCosine(
		Player->Get_DrillDistance(), PlayerDistance))
	{
		MineralMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
		return;
	}
	///
	/// if player drilling something except me -> skip.
	if (Player->Get_CurrentDrillObject_ID() != Object_ID && Player->Get_CurrentDrillObject_ID() != -1)
	{
		MineralMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
		return;
	}

	///
	/// else drill me))
	/// 
	if (Player->Get_CurrentDrillObject_Distance() == -1 || PlayerDistance <= Player->Get_CurrentDrillObject_Distance())
	{
		Player->Set_CurrentDrillObject_ID(Object_ID);
		Player->Set_CurrentDrillObject_Distance(PlayerDistance);
		MineralMesh->SetWorldScale3D(FVector(1.3f, 1.3f, 1.3f));
	}*/
}

float AMineralObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Endurance: %f"), Endurance));
	Endurance -= DamageAmount;
	if (Endurance <= 0)
	{
		int AmountCounter = 0;
		/// TODO: spawn collectable objects-  amount = MineralsAmount; *object*->cost = MineralCost;
		/// TODO: change Class of Ore to spawn basic on MiningObject(this->OreType)
		for (int i = 0; i < 1 + floor(pow(this->MineralsAmount, 1. / 3)); i++)
		{
			for (int j = 0; j < 1 + floor(pow(this->MineralsAmount, 1. / 3)); j++)
			{
				for (int k = 0; k < 1 + floor(pow(this->MineralsAmount, 1. / 3)); k++)
				{
					if (AmountCounter < MineralsAmount)
					{
						AmountCounter += 1;
						const float Shift = (1 + floor(pow(this->MineralsAmount, 1. / 3)));
						OreSpawner(this->GetActorLocation() + 50 * FVector(
							k, j, i));
					}
				}
			}
		}
		Destroy();
		/// -----
	}
	return DamageAmount;
}

void AMineralObject::OreSpawner(const FVector& Location) const
{
	const FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	if (!GetWorld())
	{
		return;
	}
	if (!OreClass)
	{
		return;
	}
	AMineralOre* f = GetWorld()->SpawnActor<AMineralOre>(OreClass, SpawnParameters);
	if (!f)
	{
		return;
	}
	f->SetActorLocation(Location);
	f->Set_Cost(this->MineralCost);
	const float RandomX = FMath::RandRange(-1.0f, 1.0f);
	const float RandomY = FMath::RandRange(-1.0f, 1.0f);
	const float RandomZ = FMath::RandRange(1.0f, 1.0f);

	f->AddForce(FMath::RandRange(150.f, 200.f), FVector(RandomX, RandomY, 0));
}

// Called every frame
void AMineralObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Player = GetPlayer();
	Timer += DeltaTime;
	if (Timer >= 0.5f && Endurance > 0.1)
	{
		TakeDrillDamage();
		Timer = 0;
	}
}

ATycho_Character* AMineralObject::GetPlayer() const
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
