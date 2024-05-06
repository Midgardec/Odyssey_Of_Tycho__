// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
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

	
	
	void LoadQuestionsFromJSON(const FString& JsonFilePath, FString& outMessage);

	
public:

	UFUNCTION(BlueprintCallable)
	void GetQuestionByTheme(const FString& Theme, ATestQuestion*& outQuestion, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	ATestQuestion* GetQuestionPtrByTheme(const FString& Theme, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void AddQuestion(ATestQuestion* newQuestion, FString& outMessage);

	void SetCurrentQuestion(ATestQuestion* newCurrent, FString& outMessage);
	bool CheckRightAnswer(const int& inIndex, FString& outMessage);

	void QuestionSolved(ATestQuestion* Question, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void GetCurrentQuestion(ATestQuestion*& outQuestion, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetCurrentTheme(const FString& Theme, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetCurrentTheme(FString& Theme, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void GetQuestions(TArray<ATestQuestion*>& outQuestions, FString& outMessage);

	static TObjectPtr<ATestQuestion> ConstructQuestion(const FString& newQuestion, const FString& newTheme, const FString& newParagraph, const TArray<FString>& newAnswers, int newRightAnswerIndex);
private:
	TArray<TObjectPtr<ATestQuestion>> m_Questions;
	FString m_Theme;
	TObjectPtr<ATestQuestion> currentQuestion;
};
