// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestQuestion.generated.h"


#define DEFAULT_STR TEXT("_")
#define MESSAGE_OK TEXT("OK")

UCLASS()
class ODYSSEY_OF_TYCHO___API ATestQuestion : public AActor
{
	GENERATED_BODY()
	
public:	
	static FString FormatErrorMessage(const FString& ErrorMessage);
	// Sets default values for this actor's properties
	ATestQuestion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UFUNCTION(BlueprintCallable)
	void GetQuestion(FString& outString, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetTheme(FString& outString, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void GetParagraph(FString& outString, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void GetAnswers(TArray<FString>& outArray, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetRightAnswerIndex(int& outIndex, FString& outMessage);
	UFUNCTION(BlueprintCallable)
	void GetStatus(bool& outStatus);
	UFUNCTION(BlueprintCallable)
	void SetQuestion(const FString& newQ, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetTheme(const FString& newTheme, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetStatus(bool newStatus, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetAnswers(const TArray<FString>& Answers, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetRightAnswerIndex(int newIndex, FString& outMessage);

	UFUNCTION(BlueprintCallable)
	void SetParagraph(const FString& newParagraph, FString& outMessage);
private:
	bool m_Status = false;// 1 - solved; 0 - unsolved;
	FString m_Theme = DEFAULT_STR;
	FString m_Question = DEFAULT_STR;
	FString m_Paragraph = DEFAULT_STR;
	TArray<FString> m_Answers = {};
	int m_RightAnswerIndex = 0;
};
