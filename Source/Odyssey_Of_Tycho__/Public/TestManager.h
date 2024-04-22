// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestManager.generated.h"
class ATestQuestion;
UCLASS()
class ODYSSEY_OF_TYCHO___API ATestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void GetQuestionByTheme(const FString& Theme, ATestQuestion* outQuestion, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void AddQuestion(ATestQuestion* newQuestion, FString& outMessage);
	void GetQuestions(TArray<ATestQuestion*>& outQuestions, FString& outMessage);
	void ConstructQuestion(const FString& newQuestion, const FString& newTheme, const TArray<FString>& newAnswers, int newRightAnswerIndex);
private:
	TArray<ATestQuestion*> m_Questions;
};
