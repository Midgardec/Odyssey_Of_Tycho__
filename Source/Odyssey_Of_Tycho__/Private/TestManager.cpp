// Fill out your copyright notice in the Description page of Project Settings.


#include "TestManager.h"
#include "JsonUtilities.h"
#include "fstream"
#include "../../../Source/Odyssey_Of_Tycho__/single_include/nlohmann/json.hpp"
#include "TestQuestion.h"

using json = nlohmann::json;

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
	FString outMessage;
	LoadQuestionsFromJSON(TEXT("Source/Odyssey_Of_Tycho__/Public/Questions.json"), outMessage);
	AddQuestion(ConstructQuestion(TEXT("How much gas does Spaceship needs to leave planet?"), TEXT("Spaceship"), L"BOBA ABOB", {"10 tons", "20 tons", "depends on it's mass"}, 2), outMessage);
	AddQuestion(ConstructQuestion(TEXT("How much does Spaceship cost?"), TEXT("Spaceship"), L"BOBA ABOBBOBA ABOBBOBA ABOB", { "1'000 $", "20'000 $", "1'000'000 $" }, 2), outMessage);
}

void ATestManager::LoadQuestionsFromJSON(const FString& JsonFilePath, FString& outMessage)
{
	FString ProjectDir = FPaths::ProjectDir();
	FString fullJsonFilePath = FPaths::Combine(ProjectDir, *JsonFilePath);
	
	std::ifstream file(*fullJsonFilePath);
	if (!file.is_open()) {
		outMessage = ATestQuestion::FormatErrorMessage(TEXT("Error: JSON: couldn't open file"));
		return;
	}
	outMessage = ATestQuestion::FormatErrorMessage(TEXT("Opened JSON successfully"));
	json data;
	file >> data;
	for (const auto& item : data) {
		TArray<FString> answers_l = {};
		for (const auto& answer : item["answers"]) {
			answers_l.Add(answer.get<std::string>().c_str());
		}
		FString outMessage_l;
		AddQuestion(ConstructQuestion(item["question"].get<std::string>().c_str(), item["theme"].get<std::string>().c_str(), item["paragraph"].get<std::string>().c_str(),
							answers_l, item["right_answer_index"]), outMessage_l);
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Cyan, item["theme"].get<std::string>().c_str());
	}
	FString JsonFileData;
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Cyan, FString::Printf(TEXT("Function called to Open JSON")));
}

void ATestManager::GetQuestionByTheme(const FString& Theme, ATestQuestion*& outQuestion, FString& outMessage)
{
	TArray<TObjectPtr<ATestQuestion>> qs = {};
	FString outMessage_l = DEFAULT_STR;
	GetQuestions(qs, outMessage);
	for (TObjectPtr<ATestQuestion> q : qs) {
		if (q != nullptr) {
			FString t_Theme = DEFAULT_STR;
			FString outMessage_l2 = DEFAULT_STR;
			bool status = false;
			q->GetTheme(t_Theme, outMessage_l2);
			if (t_Theme == DEFAULT_STR) {
				continue;
			}
			q->GetStatus(status);
			if (t_Theme == Theme && !status) {
				outQuestion = q;
				outMessage = ATestQuestion::FormatErrorMessage(MESSAGE_OK);

				return;
			}
		}
	}
	outMessage = ATestQuestion::FormatErrorMessage(FString::Printf(TEXT("error: GetQuestionByTheme: No unsolved Question with theme \"%s\""), *Theme));
	return;

}
ATestQuestion* ATestManager::GetQuestionPtrByTheme(const FString& Theme, FString& outMessage)
{
	TArray<TObjectPtr<ATestQuestion>> qs = {};
	FString outMessage_l = DEFAULT_STR;
	GetQuestions(qs, outMessage);
	for (TObjectPtr<ATestQuestion> q : qs) {
		if (q != nullptr) {
			FString t_Theme = DEFAULT_STR;
			FString outMessage_l2 = DEFAULT_STR;
			bool status = false;
			q->GetTheme(t_Theme, outMessage_l2);
			if (t_Theme == DEFAULT_STR) {
				continue;
			}
			q->GetStatus(status);
			if (t_Theme == Theme && !status) {

				outMessage = ATestQuestion::FormatErrorMessage(MESSAGE_OK);
				return q;
			}
		}
	}
	outMessage = ATestQuestion::FormatErrorMessage(FString::Printf(TEXT("error: GetQuestionByTheme: No unsolved Question with theme \"%s\""), *Theme));
	return nullptr;

}
void ATestManager::AddQuestion(ATestQuestion* newQuestion, FString& outMessage)
{
	if (!newQuestion) {
		outMessage = ATestQuestion::FormatErrorMessage(TEXT("error: AddQuestion: Invalid Question"));
		return;
	}
	m_Questions.Add(newQuestion);
	outMessage = ATestQuestion::FormatErrorMessage(MESSAGE_OK);

	return;
}

void ATestManager::SetCurrentQuestion(ATestQuestion* newCurrent, FString& outMessage)
{
	if (!newCurrent) {
		outMessage = ATestQuestion::FormatErrorMessage(TEXT("warning: Current question was set to NULL (maybe newCurrent was invalid)"));
	}
	currentQuestion = newCurrent;
	if (currentQuestion && currentQuestion == newCurrent) {
		outMessage = ATestQuestion::FormatErrorMessage(MESSAGE_OK);
	}
}

bool ATestManager::CheckRightAnswer(const int& inIndex, FString& outMessage)
{
	int rightIndex = 0;
	FString outMessage_l;
	if (!currentQuestion) {
		outMessage = ATestQuestion::FormatErrorMessage(FString::Printf(TEXT("error : CheckRightAnswer: no currentQuestion to check")));
		return 0;
	}
	currentQuestion->GetRightAnswerIndex(rightIndex, outMessage_l);
	outMessage = ATestQuestion::FormatErrorMessage(MESSAGE_OK);
	if (outMessage_l != MESSAGE_OK) {
		outMessage = ATestQuestion::FormatErrorMessage(FString::Printf(TEXT("error : RightAnswerIndex: GET - %s"), *outMessage_l));
		return 0;
	}
	if (rightIndex == inIndex) {
		QuestionSolved(currentQuestion, outMessage_l);
		return 1;
	}
	return 0;
}

void ATestManager::QuestionSolved(ATestQuestion* Question, FString& outMessage)
{
	Question->SetStatus(true, outMessage);
	currentQuestion = nullptr;
}

void ATestManager::GetCurrentQuestion(ATestQuestion*& outQuestion, FString& outMessage)
{
	if (currentQuestion) {
		outQuestion = currentQuestion;
		outMessage = ATestQuestion::FormatErrorMessage(MESSAGE_OK);
		return;
	}
	outQuestion = nullptr;
	outMessage = ATestQuestion::FormatErrorMessage(TEXT("error: CurrentQuestion: GET - no currentQuestion"));
}

void ATestManager::SetCurrentTheme(const FString& newTheme, FString& outMessage)
{

	outMessage = (newTheme != "") ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) :
		ATestQuestion::FormatErrorMessage(TEXT("error: Theme: SET - theme.len() < 0"));

	if (outMessage == ATestQuestion::FormatErrorMessage(MESSAGE_OK))
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, newTheme);
		m_Theme = newTheme;
		ATestQuestion* newQ;
		FString outM; FString outM_2;
		GetQuestionByTheme(m_Theme, newQ, outM);
		if (outM == ATestQuestion::FormatErrorMessage(MESSAGE_OK)) {
			SetCurrentQuestion(newQ, outM_2);
		}
		else {
			SetCurrentQuestion(nullptr, outM_2);
		}
	}
}

void ATestManager::GetCurrentTheme(FString& Theme, FString& outMessage)
{
	Theme = m_Theme;
	outMessage = (Theme.Len() > 0) ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) :
		ATestQuestion::FormatErrorMessage(TEXT("error: Theme: GET - No Theme in question"));

}

void ATestManager::GetQuestions(TArray<ATestQuestion*>& outQuestions, FString& outMessage)
{
	if (m_Questions.Num() > 0) {
		outQuestions = m_Questions;
		outMessage = ATestQuestion::FormatErrorMessage(MESSAGE_OK);
	}
	else {
		outMessage = ATestQuestion::FormatErrorMessage(TEXT("Error in GetQuestions: No questions"));
	}

}

TObjectPtr<ATestQuestion> ATestManager::ConstructQuestion(const FString& newQuestion, const FString& newTheme, const FString& newParagraph, const TArray<FString>& newAnswers, int newRightAnswerIndex)
{
	TObjectPtr<ATestQuestion> Question = NewObject<ATestQuestion>();
	FString outMessage;
	Question->SetQuestion(newQuestion, outMessage); // TODO: check outMessage
	Question->SetTheme(newTheme, outMessage); // TODO: check outMessage
	Question->SetParagraph(newParagraph, outMessage);// TODO: check outMessage
	Question->SetStatus(false, outMessage); // TODO: check outMessage
	Question->SetAnswers(newAnswers, outMessage); // TODO: check outMessage
	Question->SetRightAnswerIndex(newRightAnswerIndex, outMessage); // TODO: check outMessage
	return Question;
}

