// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelManager.h"
#include "PlanetBase.h"
#include "Spaceship.h"

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

}

// Called every frame
void ATravelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_FocusedPlanet && m_Spaceship) {
		FString mes_dis;
		FString mes_vel;
		CalculateDistance(mes_dis);
		CalculateVelocity(mes_vel);
	}
}


void ATravelManager::SetFocusedPlanet(APlanetBase*& newFocusedPlanet, FString& OutMessage) {
	if (m_FocusedPlanet) {
		m_FocusedPlanet = newFocusedPlanet;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: FocusedPlanet: SET - newFocusedPlanet is not valid.");
	}
}

void ATravelManager::CalculateDistance(FString& OutMessage) {
	if (m_FocusedPlanet && m_Spaceship) {
		FVector PlanetLocation = m_FocusedPlanet->GetActorLocation();
		FVector PlayerLocation = m_Spaceship->GetActorLocation();
		m_Distance = FVector::Distance(PlanetLocation, PlayerLocation);;
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
		m_Velocity = FVector::DotProduct(PlayerVelocity.GetSafeNormal(), PlayerLocation - PlanetLocation);
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: Velocity: CALC - m_FocusedPlanet or m_Player is not valid.");
	}
}

void ATravelManager::SetSpaceship(ASpaceship* newSpaceship, FString& OutMessage) {
	if (m_Spaceship) {
		m_Spaceship= newSpaceship;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: player: SET - inPlayer is not valid.");
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

void ATravelManager::GetSpaceshipVelocity(float& outVelocity, FString& OutMessage)
{
	if (m_Spaceship) {
		outVelocity = m_Velocity;
		OutMessage = FormatErrorMessage(MESSAGE_OK);
	}
	else {
		OutMessage = FormatErrorMessage(L"Error: Velocity: GET - there is no spaceship");
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
