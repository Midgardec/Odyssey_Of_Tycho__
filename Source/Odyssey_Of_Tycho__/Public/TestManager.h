// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Actor.h"
#include "TestManager.generated.h"

class ATestQuestion;
class ATycho_Character;

#define RIGHT_ANSWER_COINS 20
#define WRONG_ANSWER_COINS -5

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
	void GetParagraphByTheme(const FString& Theme, ATestParagraph*& outParagraph, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	ATestQuestion* GetQuestionPtrByTheme(const FString& Theme, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void AddQuestion(ATestQuestion* newQuestion, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void AddParagraph(ATestParagraph* newParagraph, FString& outMessage);

	void SetCurrentQuestion(TObjectPtr<ATestQuestion> newCurrent, FString& outMessage);
	void CheckRightAnswer(int inIndex, FString& outMessage);

	ATycho_Character* Get_Player() const;
	void Set_Player(ATycho_Character* Player_);

	void QuestionSolved(TObjectPtr<ATestQuestion> Question, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void GetCurrentQuestion(ATestQuestion*& outQuestion, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetCurrentTheme(const FString& Theme, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetCurrentTheme(FString& Theme, FString& outMessage) const;

	UFUNCTION(BlueprintCallable)
	void GetQuestions(TArray<ATestQuestion*>& outQuestions, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetParagraphs(TArray<ATestParagraph*>& outQuestions, FString& outMessage);

	static TObjectPtr<ATestQuestion> ConstructQuestion(const FString& newQuestion, const FString& newTheme, const TArray<FString>& newAnswers, int newRightAnswerIndex, TObjectPtr<ATestParagraph> newParagraph);
	static TObjectPtr<ATestParagraph> ConstrucParagraph(const FString& newParagraph, const FString& newParagraphFull, const FString& newTheme);
private:
	UPROPERTY()
	TArray<TObjectPtr<ATestQuestion>> m_Questions; 
	UPROPERTY()
	TArray<TObjectPtr<ATestParagraph>> m_Paragraphs;
	FString m_Theme;
	UPROPERTY()
	TObjectPtr<ATestQuestion> currentQuestion;

	UPROPERTY()
	TObjectPtr<ATycho_Character> m_Player;
};
