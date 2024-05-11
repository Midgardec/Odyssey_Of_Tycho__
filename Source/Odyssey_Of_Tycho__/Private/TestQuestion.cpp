// Fill out your copyright notice in the Description page of Project Settings.


#include "TestQuestion.h"
#include "TestParagraph.h"

FString ATestQuestion::FormatErrorMessage(const FString& ErrorMessage)
{
	return FString::Printf(TEXT("%s"), *ErrorMessage);
}
// Sets default values
ATestQuestion::ATestQuestion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	m_Answers.Init(DEFAULT_STR, 4);

}

// Called when the game starts or when spawned
void ATestQuestion::BeginPlay()
{
	Super::BeginPlay();

}


void ATestQuestion::GetQuestion(FString& outString, FString& outMessage)
{
		outString = (m_Question.Len() > 0) ? m_Question : "";
		outMessage = (outString.Len() > 0) ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: Question: SET - No question"));


}

void ATestQuestion::GetTheme(FString& outString, FString& outMessage)
{

	outString = (m_Theme.Len() > 0) ? m_Theme : "";
	outMessage = (outString.Len() > 0) ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: Theme: GET - No Theme in question"));

}

//void ATestQuestion::GetParagraph(FString& outString, FString& outMessage)
//{
//	outString = m_Paragraph;
//	outMessage = (outString.Len() > 0) ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: Paragraph: GET - No Paragraph in question"));
//}

void ATestQuestion::GetAnswers(TArray<FString>& outArray, FString& outMessage)
{

	outArray = (m_Answers.Num() > 0) ? m_Answers : TArray<FString>();
	outMessage = (outArray.Num() > 0) ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: AnswersArray: GET - No answers"));

}

void ATestQuestion::GetRightAnswerIndex(int& outIndex, FString& outMessage)
{
	outIndex = m_RightAnswerIndex;
	outMessage = (outIndex >= 0 && outIndex< m_Answers.Num()) ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: RightAnswerIndex: GET - InvalidIndex"));

}

void ATestQuestion::GetStatus(bool& outStatus)
{
	outStatus = m_Status;
}

void ATestQuestion::SetQuestion(const FString& newQ, FString& outMessage)
{
	outMessage = (newQ != "") ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: Question: GET - question.len() < 0"));
	if (outMessage == FormatErrorMessage(MESSAGE_OK))
	{
		m_Question = newQ;
	}
}

void ATestQuestion::SetTheme(const FString& newTheme, FString& outMessage)
{
	outMessage = (newTheme != "") ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: Theme: SET - theme.len() < 0"));
	if (outMessage == FormatErrorMessage(MESSAGE_OK))
	{
		m_Theme = newTheme;
	}
}

void ATestQuestion::SetStatus(bool newStatus, FString& outMessage)
{
	m_Status = newStatus;
	outMessage = FormatErrorMessage(MESSAGE_OK);
}

void ATestQuestion::SetAnswers(const TArray<FString>& Answers, FString& outMessage)
{
	outMessage = (Answers.Num() > 0) ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: AnswersArray: SET - no items in array"));
	if (outMessage == FormatErrorMessage(MESSAGE_OK))
	{
		m_Answers = Answers;
	}

}

void ATestQuestion::SetRightAnswerIndex(int newIndex, FString& outMessage)
{
	outMessage = (m_Answers.IsValidIndex(newIndex)) ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: RightAnswerIndex: SET - invalid index"));
	if (outMessage == FormatErrorMessage(MESSAGE_OK))
	{
		m_RightAnswerIndex = newIndex;
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, FString::Printf(TEXT("%i"), m_RightAnswerIndex));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, outMessage);
}

void ATestQuestion::SetParagraphPtr(TObjectPtr<ATestParagraph> newParagraph, FString& outMessage)
{
	if (!newParagraph) {
		outMessage = FormatErrorMessage(FString::Printf(TEXT("_error : ParagraphPtr: SET - invalid Paragraph pointer")));
	}
	m_ParagraphPtr = newParagraph;
	outMessage = FormatErrorMessage(MESSAGE_OK);
}
void ATestQuestion::GetParagraphPtr(ATestParagraph*& outParagraph, FString& outMessage)
{
	if (!m_ParagraphPtr) {
		outMessage = FormatErrorMessage(FString::Printf(TEXT("_error : ParagraphPtr: GET - invalid Paragraph pointer")));
		
	}
	outParagraph = m_ParagraphPtr;
	outMessage = FormatErrorMessage(MESSAGE_OK);
}

//void ATestQuestion::SetParagraph(const FString& newParagraph, FString& outMessage)
//{
//	outMessage = (newParagraph != "") ? FormatErrorMessage(MESSAGE_OK) : FormatErrorMessage(TEXT("error: Paragraph: SET - Paragraph.len() < 0"));
//	if (outMessage == FormatErrorMessage(MESSAGE_OK))
//	{
//		m_Paragraph = newParagraph;
//	}
//}

