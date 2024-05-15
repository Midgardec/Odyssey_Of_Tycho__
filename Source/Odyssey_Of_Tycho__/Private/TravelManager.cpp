// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelManager.h"
#include "PlanetBase.h"
#include "Spaceship.h"
#include <Kismet/GameplayStatics.h>

FString ATravelManager::FormatErrorMessage(const FString& ErrorMessage)
{
	return FString::Printf(TEXT("%s"), *ErrorMessage);
}

// Sets default values
ATravelManager::ATravelManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATravelManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> Planets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanetBase::StaticClass(), Planets);
	FString MES_SET_FCSD_PLNT;

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(L"amount of planets %i", Planets.Num()));
	for (auto p : Planets) {
		if (p) {
			APlanetBase* p_l = StaticCast<APlanetBase*>(p);
			this->SetFocusedPlanet(p_l, MES_SET_FCSD_PLNT);
			break;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, MES_SET_FCSD_PLNT);
}

// Called every frame
void ATravelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_FocusedPlanet && m_Spaceship && m_Spaceship->GetController()) {
		/*GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Emerald,
			FString::Printf(L"there is focused planet %s and possessed spaceship and distance/ velocity is: %f / %f |====| H: %f \n V:%f ",
				*m_FocusedPlanet->Name, m_Distance, m_ApproachSpeed, m_HorizontalVelocity.Size(), m_VerticalVelocity.Size()));
		*/
		FString mes_dis;
		FString mes_vel;
		CalculateDistance(mes_dis);
		CalculateVelocity(mes_vel);

		
		if (m_Distance < ( m_FocusedPlanet->Resolution/2 )) {
			FString mes_set_FCSD_PLNT;
			SetFocusedPlanet(nullptr, mes_set_FCSD_PLNT);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, mes_set_FCSD_PLNT);
			/// TODO: Potencialy - place for PLANET_TYPE event
		}
	}
}


void ATravelManager::SetFocusedPlanet(APlanetBase* newFocusedPlanet, FString& OutMessage) {
	//if (newFocusedPlanet) {
	m_FocusedPlanet = newFocusedPlanet;
	OutMessage = FormatErrorMessage(MESSAGE_OK);
	//}
	//else {
	//	OutMessage = FormatErrorMessage(L"Error: FocusedPlanet: SET - newFocusedPlanet is not valid.");
	//}
}
void ATravelManager::SetSpaceship(ASpaceship* newSpaceship, FString& OutMessage) {
	//if (newSpaceship) {
	m_Spaceship = newSpaceship;
	OutMessage = FormatErrorMessage(MESSAGE_OK);
	//}
	//else {
	//	OutMessage = FormatErrorMessage(L"Error: Spaceship: SET - newSpaceship is not valid.");
	//}
}

void ATravelManager::CalculateDistance(FString& OutMessage) {
	if (m_FocusedPlanet && m_Spaceship) {
		FVector PlanetLocation = m_FocusedPlanet->GetActorLocation();
		FVector PlayerLocation = m_Spaceship->GetActorLocation();
		m_Distance = FVector::Distance(PlanetLocation, PlayerLocation) - (m_FocusedPlanet->Radius);
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: Distance: CALC -  m_FocusedPlanet or m_Player is not valid.");
	}
}

void ATravelManager::CalculateVelocity(FString& OutMessage) {
	if (m_FocusedPlanet && m_Spaceship) {
		FVector PlanetLocation = m_FocusedPlanet->GetActorLocation();
		FVector PlayerLocation = m_Spaceship->GetActorLocation();
		FVector PlayerVelocity = m_Spaceship->GetVelocity();
		// Вектор, направленный от планеты к кораблю
		FVector PlanetToPlayer = PlanetLocation - PlayerLocation;

		FVector UpVector = m_Spaceship->GetActorUpVector();
		FVector RightVector = m_Spaceship->GetActorRightVector();

		FVector VerticalDirection = FVector::CrossProduct(PlanetToPlayer, UpVector).GetSafeNormal(); // Получаем вертикальное направление относительно корабля
		FVector HorizontalDirection = FVector::CrossProduct(PlanetToPlayer, RightVector).GetSafeNormal();

		// Горизонтальная скорость (векторное произведение вектора скорости и вектора, направленного от планеты к кораблю)
		m_HorizontalVelocity = PlayerVelocity.ProjectOnTo(HorizontalDirection);
		// Вертикальная скорость (проекция скорости космического корабля на вектор, направленный от планеты к кораблю)
		m_VerticalVelocity = PlayerVelocity.ProjectOnTo(VerticalDirection);

		// Скорость сближения (проекция скорости космического корабля на вектор, направленный от планеты к кораблю)
		m_ApproachSpeed = FVector::DotProduct(PlanetToPlayer.GetSafeNormal(), PlayerVelocity);
		//m_Velocity = FVector::DotProduct(PlayerVelocity.GetSafeNormal(), PlanetLocation - PlayerLocation);
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: Velocity: CALC - m_FocusedPlanet or m_Player is not valid.");
	}
}


void ATravelManager::SetPermissionToFlight(bool OutPermission, FString& OutMessage) {
	m_PermissionToFlight = OutPermission;
	OutMessage = FormatErrorMessage(MESSAGE_OK);
}

void ATravelManager::GetDistance(float& outDistance, FString& OutMessage)
{
	if (m_FocusedPlanet) {
		outDistance = m_Distance;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: distance: GET - there is no planet in focus");
	}

}

void ATravelManager::GetSpaceshipApproachSpeed(float& outApproachSpeed, FString& OutMessage)
{
	if (m_Spaceship) {
		outApproachSpeed = m_ApproachSpeed;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: Velocity: GET - there is no spaceship");
	}
}

void ATravelManager::GetSpaceshipHorizontalVelocity(FVector& HorizontalVelocity, FString& OutMessage)
{
	if (m_Spaceship) {
		HorizontalVelocity = m_HorizontalVelocity;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: HorizontalVelocity: GET - there is no spaceship");
	}
}

void ATravelManager::GetSpaceshipVerticalVelocity(FVector& VerticalVelocity, FString& OutMessage)
{
	if (m_Spaceship) {
		VerticalVelocity = m_VerticalVelocity;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: VerticalVelocity: GET - there is no spaceship");
	}
}

void ATravelManager::GetFocusedPlanet(APlanetBase*& FocusedPlanet, FString& OutMessage)
{
	if (m_FocusedPlanet) {
		FocusedPlanet = m_FocusedPlanet;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: FocusedPlanet: GET - there is no planet in focus");
	}
}

void ATravelManager::GetPlayerSpaceship(ASpaceship*& Spaceship, FString& OutMessage)
{
	if (m_Spaceship) {
		Spaceship = m_Spaceship;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: Spaceship: GET - there is no spaceship");
	}
}

bool ATravelManager::GetPermissionToFlight()
{
	return m_PermissionToFlight;
}

void ATravelManager::GetFocusedPlanetName(FString& FocusedPlanetName, FString& OutMessage)
{
	if (m_FocusedPlanet) {
		FocusedPlanetName = m_FocusedPlanet->Name;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		//FocusedPlanetName = DEFAULT_STR;
		OutMessage = FormatErrorMessage(L"Error: FocusedPlanetName: GET - there is no planet in focus");
	}
}
