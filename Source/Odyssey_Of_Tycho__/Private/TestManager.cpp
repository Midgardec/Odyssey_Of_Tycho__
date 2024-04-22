// Fill out your copyright notice in the Description page of Project Settings.


#include "TestManager.h"
#include "TestQuestion.h"
// Sets default values
ATestManager::ATestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestManager::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATestManager::GetQuestionByTheme(const FString& Theme, ATestQuestion* outQuestion, FString& outMessage)
{
	TArray<ATestQuestion*> qs;
	FString outMessage;
	GetQuestions(qs, outMessage);
	for (ATestQuestion* q : qs) {
		if (q) {
			FString t_Theme = DEFAULT_STR;
			FString outMessage_l = DEFAULT_STR;
			bool status = false;
			q->GetTheme(t_Theme, outMessage_l);
			if (t_Theme != DEFAULT_STR) {
				continue;
			}
			q->GetStatus(status);
			if (t_Theme == Theme && !status) {
				outQuestion = q;
				outMessage = FString::Printf(TEXT('OK'));

				return ;
			}
		}
	}
	outMessage = FString::Printf(TEXT('No Question with theme / unsolved % s'), Theme);
	return;

}
void ATestManager::AddQuestion(ATestQuestion* newQuestion, FString& outMessage)
{
	if (!newQuestion) {
		outMessage = FString::Printf(TEXT('Invalid Question'));
		return;
	}
	m_Questions.Add(newQuestion);
	outMessage = FString::Printf(TEXT('OK'));

	return;
}

void ATestManager::GetQuestions(TArray<ATestQuestion*>& outQuestions, FString& outMessage)
{
	if (m_Questions.Num() > 0) {
		outQuestions = m_Questions;
		outMessage = FString::Printf(TEXT("OK"));
	}
	else {
		outMessage = FString::Printf(TEXT("No questions"));
	}
	
}

void ATestManager::ConstructQuestion(const FString& newQuestion, const FString& newTheme, const TArray<FString>& newAnswers, int newRightAnswerIndex)
{
	ATestQuestion* Question = NewObject<ATestQuestion>();
	FString outMessage;
	Question->SetQuestion(newQuestion, outMessage); // TODO: check outMessage
	Question->SetTheme(newTheme, outMessage); // TODO: check outMessage
	Question->SetStatus(false, outMessage); // TODO: check outMessage
	Question->SetAnswers(newAnswers, outMessage); // TODO: check outMessage
	Question->SetRightAnswerIndex(newRightAnswerIndex, outMessage); // TODO: check outMessage
}

