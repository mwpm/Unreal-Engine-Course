#pragma once
#include "FBullCowGame.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>

// to make syntax Unreal friendly
#define TMap std::map
#define FVector std::vector
#define TStream std::ifstream // not sure if Unreal syntax
using int32 = int;


int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries{ {2, 3}, {3, 4}, {4, 7}, {5, 10}, {6, 16}, {7, 20}, {8,30} };
	return WordLengthToMaxTries[MyHiddenWord.length()]; 
}

FBullCowGame::FBullCowGame()
{
	Reset();
	CreateWordLengthToStringsMap();
}

FBullCowGame::~FBullCowGame()
{
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; // default isogram
	MyHiddenWord = HIDDEN_WORD; // this MUST be an isogram
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess))
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength())
	{
		return EGuessStatus::Incorrect_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

EHiddenWordStatus FBullCowGame::SetHiddenWordWithLength(int32 length)
{
	if (length == 0)
	{
		return EHiddenWordStatus::OK;
	}
	else if(MyWordLengthToStrings.find(length) == MyWordLengthToStrings.end())
	{
		return EHiddenWordStatus::Invalid_Hidden_Word;
	}

	// Get random word in list of words of given length
	srand(time(NULL));
	int32 NumberOfStrings = MyWordLengthToStrings[length].size();
	int32 RandomIndex = rand() % NumberOfStrings;
	MyHiddenWord = MyWordLengthToStrings[length].at(RandomIndex);

	return EHiddenWordStatus::OK;
}

// receives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	
	int32 WordLength = MyHiddenWord.length(); // assume same length as Guess

	// loop through all letters in the guess
	for (int32 GIndex = 0; GIndex < WordLength; GIndex++)
	{
		// compare letter against the hidden word
		for (int32 MHWIndex = 0; MHWIndex < WordLength; MHWIndex++)
		{
			//	if they match
			if (Guess[GIndex] == MyHiddenWord[MHWIndex])
			{
				// if they're in the same place
				if (GIndex == MHWIndex)
				{
					BullCowCount.Bulls++;
				}
				else
				{
					BullCowCount.Cows++;
				}
			}
		}
	}

	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}

	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	int32 GuessWordLength = Word.length();

	// treat 0 and 1 letter strings as isogram
	if (GuessWordLength == 0 || GuessWordLength == 1)
	{
		return true;
	}

	// Create map
	TMap<char, bool> LetterSeen;

	// for each character 
	for (auto Letter : Word)
	{
		// if letter is in the map
		Letter = tolower(Letter);
		if (LetterSeen[Letter])
		{
			return false;
		}
		else
		{
			LetterSeen[Letter] = true;
		}
	}

	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter))
		{
			return false;
		}
	}
	
	return true;
}

void FBullCowGame::CreateWordLengthToStringsMap()
{
	TStream WordStream;
	WordStream.open("Words.txt", TStream::in);
	std::cout << "Loading game...\n";
	FString Word;
	
	// loop through all words in file and add to map
	while (std::getline(WordStream, Word))
	{
		if (IsIsogram(Word) && IsLowercase(Word))
		{
			int32 WordLength = Word.length();

			// if WordLength does not exist in map yet, create/insert word list and add word
			if (MyWordLengthToStrings.find(WordLength) == MyWordLengthToStrings.end())
			{
				FVector<FString> Strings;
				Strings.push_back(Word);
				MyWordLengthToStrings[WordLength] = Strings;
			}
			else
			{
				FVector<FString> Strings = MyWordLengthToStrings[WordLength];
				// assume no duplicates in Word.txt
				MyWordLengthToStrings[WordLength].push_back(Word); 
			}
		}
	}
}
