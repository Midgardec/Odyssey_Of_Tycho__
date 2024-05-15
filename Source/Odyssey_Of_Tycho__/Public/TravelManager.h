// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TravelManager.generated.h"
class APlanetBase;
class ASpaceship;
#define DEFAULT_STR TEXT("_")
#define MESSAGE_OK TEXT("OK")
UCLASS()
class ODYSSEY_OF_TYCHO___API ATravelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	FString FormatErrorMessage(const FString& ErrorMessage);
	// Sets default values for this actor's properties
	ATravelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/// -- var zone
private:
	UPROPERTY()
	TObjectPtr<APlanetBase> m_FocusedPlanet;
	UPROPERTY()
	float m_Distance;
	UPROPERTY()
	float m_ApproachSpeed;
	UPROPERTY()
	FVector m_HorizontalVelocity;
	UPROPERTY()
	FVector m_VerticalVelocity;

	UPROPERTY()
	TObjectPtr<ASpaceship> m_Spaceship;
	UPROPERTY()
	bool m_PermissionToFlight;

public:
	UFUNCTION(BlueprintCallable)
	void SetFocusedPlanet(APlanetBase* OutFocusedPlanet, FString& OutMessage);

	UFUNCTION(BlueprintCallable)
	void CalculateDistance(FString& OutMessage);

	UFUNCTION(BlueprintCallable)
	void CalculateVelocity(FString& OutMessage);
	
	UFUNCTION(BlueprintCallable)
	void SetSpaceship(ASpaceship* newSpaceship, FString& OutMessage);

	UFUNCTION(BlueprintCallable)
	void SetPermissionToFlight(bool newPermission, FString& OutMessage);

	UFUNCTION(BlueprintCallable)
	void GetDistance(float& Distance, FString& OutMessage);
	UFUNCTION(BlueprintCallable)
	void GetSpaceshipApproachSpeed(float& ApproachSpeed, FString& OutMessage);
	UFUNCTION(BlueprintCallable)
	void GetSpaceshipHorizontalVelocity(FVector& HorizontalVelocity, FString& OutMessage);
	UFUNCTION(BlueprintCallable)
	void GetSpaceshipVerticalVelocity(FVector& VerticalVelocity, FString& OutMessage);
	UFUNCTION(BlueprintCallable)
	void GetFocusedPlanet(APlanetBase*& FocusedPlanet, FString& OutMessage);
	UFUNCTION(BlueprintCallable)
	void GetPlayerSpaceship(ASpaceship*& Spaceship, FString& OutMessage);
	UFUNCTION(BlueprintCallable)
	bool GetPermissionToFlight();
	UFUNCTION(BlueprintCallable)
	void GetFocusedPlanetName(FString& FocusedPlanetName, FString& OutMessage);

};
