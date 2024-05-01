// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestManager.h"
#include "GameFramework/Actor.h"
#include "TSEventManager.generated.h"

UCLASS()
class ODYSSEY_OF_TYCHO___API ATSEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATSEventManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ProcessEventType(const EEventType& EventType);
public:
	EEventType m_EventType = EEventType::EventNull;
	TObjectPtr<ATestManager> m_TestManager= nullptr;
};
