// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBoxBase.h"

// Sets default values
ALootBoxBase::ALootBoxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALootBoxBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootBoxBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


