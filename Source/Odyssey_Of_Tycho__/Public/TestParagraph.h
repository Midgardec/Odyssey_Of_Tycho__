// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestQuestion.h"
#include "TestParagraph.generated.h"


UCLASS()
class ODYSSEY_OF_TYCHO___API ATestParagraph : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestParagraph();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void GetParagraph(FString& outString, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetFullParagraph(FString& outString, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetTheme(FString& outString, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetParagraph(const FString& newParagraph, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void SetFullParagraph(const FString& newParagraph, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void SetTheme(const FString& newTheme, FString& outMessage);

public:
	FString m_Paragraph = DEFAULT_STR;
	FString m_Paragraph_full = DEFAULT_STR;
	FString m_Theme = DEFAULT_STR;

};
