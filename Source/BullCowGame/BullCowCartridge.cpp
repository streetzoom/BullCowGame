// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(HWords);
    Score = 0; // updCode
    
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput)   // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);       
    } 
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming The Player
    PrintLine(TEXT("Welcome in BullCowGame!"));
     
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() -1)]; 
    Lives = HiddenWord.Len() * 2;
  
    bGameOver = false;
    PrintLine(TEXT("You need to guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Just 2 simple rules \n1:Bull is correct letter and position \n2:Cow is correct letter but wrong position"));
    //PrintLine(TEXT("The HIddenWord is: %s."), *HiddenWord); // Debug line

    PrintLine(TEXT("Your Score is: %i"), Score);
    PrintLine(TEXT("You have %i Lives."), Lives);

    PrintLine(TEXT("Press TAB to write \nand type in your guess")); // Prompt Player For Guess  
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPlease press enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ++Score;
        PrintLine(TEXT("You have Won!"));   
        PrintLine(TEXT("Your Score is: %i"), Score);    
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        ClearScreen(); // updCode
        PrintLine(TEXT("The hidden word is %i letters long!"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining"), Lives); 
        return;                  
    }

    // Check IF Isogram
    if (!IsIsogram(Guess))
    {
        ClearScreen(); // updCode
        PrintLine(TEXT("No repeating letters, guess again."));
        return;
    }

    // Remove Life
    PrintLine(TEXT("Lost a life!"));
    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        Score = 0;
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        PrintLine(TEXT("Better luck next time!"));
        EndGame();
        return;
    } 
    // Show the player the Bulls and Cows
    FBullCowCount Check = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows."), Check.Bulls, Check.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left."), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }          
        }      
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (auto Word : WordList)
    {
        if (Word.Len() >= 3 && Word.Len() <= 4 && IsIsogram(Word))
        {          
            ValidWords.Emplace(Word);
        }         
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }            
        }    
    }  
    return Count;
}
