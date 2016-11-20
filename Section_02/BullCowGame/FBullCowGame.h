/* This is game logic (no view code or direct user interation)
The game is a simple guess the word game based on Mastermind
*/

#pragma once

#include <string>
#include <map>
#include <vector>

// to make syntax Unreal friendly
#define TMap std::map
#define FVector std::vector
using FString = std::string;
using int32 = int;

enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Incorrect_Length,
	Not_Lowercase,
};

enum class EHiddenWordStatus
{
	Invalid_Hidden_Word,
	No_Hidden_Word,
	OK,
};

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

class FBullCowGame
{
public:
	FBullCowGame(); // constructor
	~FBullCowGame(); // deconstructor

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	bool IsGameWon() const;

	EGuessStatus CheckGuessValidity(FString) const;
	EHiddenWordStatus SetHiddenWordWithLength(int32);

	void Reset();
	FBullCowCount SubmitValidGuess(FString);

private:
	// see constructor for initialization
	int32 MyCurrentTry;
	FString MyHiddenWord;
	bool bGameIsWon;
	TMap<int32, FVector<FString>> MyWordLengthToStrings; // map of string length to vector of strings

	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
	void CreateWordLengthToStringsMap();
};