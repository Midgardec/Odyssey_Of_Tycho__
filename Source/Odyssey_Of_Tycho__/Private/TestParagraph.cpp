// Fill out your copyright notice in the Description page of Project Settings.


#include "TestParagraph.h"

// Sets default values
ATestParagraph::ATestParagraph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestParagraph::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATestParagraph::GetParagraph(FString& outString, FString& outMessage)
{
	outString = (m_Paragraph.Len() > 0) ? m_Paragraph : "";
	outMessage = (outString.Len() > 0) ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) : ATestQuestion::FormatErrorMessage(TEXT("error: Paragraph : GET - No Paragraph"));

}

void ATestParagraph::GetFullParagraph(FString& outString, FString& outMessage)
{
	outString = (m_Paragraph_full.Len() > 0) ? m_Paragraph_full : "";
	outMessage = (outString.Len() > 0) ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) : ATestQuestion::FormatErrorMessage(TEXT("error: Paragraph_full: GET - No Full Paragraph"));

}

void ATestParagraph::GetTheme(FString& outString, FString& outMessage)
{
	outString = (m_Theme.Len() > 0) ? m_Theme : "";
	outMessage = (outString.Len() > 0) ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) : ATestQuestion::FormatErrorMessage(TEXT("error: Theme: GET - No Theme in question"));
}

void ATestParagraph::SetParagraph(const FString& newParagraph, FString& outMessage)
{
	outMessage = (newParagraph != "") ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) : ATestQuestion::FormatErrorMessage(TEXT("error: Paragraph: SET - newParagraph.len() < 0"));
	if (outMessage == ATestQuestion::FormatErrorMessage(MESSAGE_OK))
	{
		m_Paragraph = newParagraph;
	}
}

void ATestParagraph::SetFullParagraph(const FString& newParagraph, FString& outMessage)
{
	outMessage = (newParagraph != "") ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) : ATestQuestion::FormatErrorMessage(TEXT("error: Paragraph_full: SET - newFullParagraph.len() < 0"));
	if (outMessage == ATestQuestion::FormatErrorMessage(MESSAGE_OK))
	{
		m_Paragraph_full = newParagraph;
	}
}

void ATestParagraph::SetTheme(const FString& newTheme, FString& outMessage)
{
	outMessage = (newTheme != "") ? ATestQuestion::FormatErrorMessage(MESSAGE_OK) : ATestQuestion::FormatErrorMessage(TEXT("error: Theme: SET - theme.len() < 0"));
	if (outMessage == ATestQuestion::FormatErrorMessage(MESSAGE_OK))
	{
		m_Theme = newTheme;
	}
}
