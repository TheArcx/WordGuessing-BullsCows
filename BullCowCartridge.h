// Fill

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FCount
{
	int32 Hits = 0;
	int32 Miss = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString &Input) override;
	void EndGame();
	void GameSetUp();
	void ParseGuess(const FString &Guess, int32 Count);
	void WinScreen();
	bool IsIsogram(const FString &HWord) const;
	TArray<FString> AuthenticateWords(const TArray<FString> &WordList) const;
	FCount HitMiss(const FString &Guess) const;

private:
	FString HiddenWord;		// The string of the hidden word
	int32 Lives;			// Attempts to guess the word
	int32 HiddenWordLength; // Length of the hidden word
	bool bGameLoop;			// Controls the main game loop
	TArray<FString> Words;	// Main string array of words
	TArray<FString> Isograms;
};
