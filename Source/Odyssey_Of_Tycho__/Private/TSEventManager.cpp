// Fill out your copyright notice in the Description page of Project Settings.


#include "TSEventManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ATSEventManager::ATSEventManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATSEventManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> TestManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestManager::StaticClass(), TestManagers);

	for (auto TM : TestManagers)
	{
		m_TestManager = Cast<ATestManager>(TM);
	}
}

// Called every frame
void ATSEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATSEventManager::ProcessEventType(const EEventType& EventType)
{
	m_EventType = EventType;
	if (!m_TestManager) {
		m_EventType = EEventType::EventNull;
		return;
	}
	FString outMessage;
	switch (m_EventType) {
		case EEventType::EventGravity: {
			m_TestManager->SetCurrentTheme("Gravity", outMessage);
			break;
		}
		case EEventType::EventStars: {
			m_TestManager->SetCurrentTheme("Stars", outMessage);
			break;
		}
		case EEventType::EventPlanets: {
			m_TestManager->SetCurrentTheme("Planets", outMessage);
			break;
		}
		case EEventType::EventMoons: {
			m_TestManager->SetCurrentTheme("Moons", outMessage);
			break;
		}
		case EEventType::EventSpaceship: {
			m_TestManager->SetCurrentTheme("Spaceship", outMessage);
			break;
		}
		case EEventType::EventNull: {
			m_TestManager->SetCurrentTheme("Gravity", outMessage);
			break;
		}
		default:
			break;
	}
}

