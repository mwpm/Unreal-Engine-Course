// Michael Molina
/* This is the console executable, that makes use of the BullCow Class
This acts as the view in a MVC pattern, and is responsible for all 
user interaction. For game logic see FBullCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
void PrintIntroduction();
FText GetValidGuess();
void PlayGame();
void AskWordLength(); //helper
bool AskToPlayAgain();
void PrintGameSummary();


FBullCowGame BCGame; // instantiate a new game, which we re-use across plays

void PrintIntroduction()
{
	std::cout << "\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << std::endl;
	return;
}

// Loop continually unitl the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do
	{
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		int32 MaxTries = BCGame.GetMaxTries();
		std::cout << "Try (" << CurrentTry << "/" << MaxTries << "). Enter your guess: ";
		std::getline(std::cin, Guess);

		// check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Incorrect_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // kee loop until we get no errors

	return Guess;
}

// Plays a single game to completion
void PlayGame()
{
	BCGame.Reset();

	AskWordLength();

	int32 MaxTries = BCGame.GetMaxTries();

	// Loop asking for guesses while the game is NOT won
	// and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess();

		// Submit valid guess to the game
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		
		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << " Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();

	return;
}

void AskWordLength()
{
	FString SLength = "";
	EHiddenWordStatus Status = EHiddenWordStatus::Invalid_Hidden_Word;
	do
	{
		std::cout << "How many letters in an isgoram do you want to guess (0 for default)? ";
		std::getline(std::cin, SLength);

		int32 Length = atoi(SLength.c_str());
		Status = BCGame.SetHiddenWordWithLength(Length);
		if (Status != EHiddenWordStatus::OK)
		{
			std::cout << "No isograms exist with that many letters.\n";
		}
		std::cout << std::endl;
	} while (Status != EHiddenWordStatus::OK);
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	return;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
	}
	else
	{
		std::cout << "Better luck next time.\n";
	}
	return;
}


int main()
{
	bool bPlayAgain = false;
	do
	{
		PrintIntroduction();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);
	return 0; // exit application
}
