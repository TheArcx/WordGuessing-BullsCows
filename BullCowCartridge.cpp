#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath); // Load word file
    Isograms = AuthenticateWords(Words);

    GameSetUp();
    PrintLine(TEXT("tHE NUMBER OF VALID WORDS IS: %i."), AuthenticateWords(Words).Num());
}

/* Runs when the player interacts with board / hits enter */
void UBullCowCartridge::OnInput(const FString &Input)
{
    if (bGameLoop != false)
    {
        ParseGuess(Input, Lives);
    }
    else
    {
        ClearScreen();
        GameSetUp();
    }
}

/* Parses the users input */
void UBullCowCartridge::ParseGuess(const FString &Guess, int32 count)
{
    if (Guess == HiddenWord) // Word guessed correctly
    {
        WinScreen();
    }
    else
    {
        --Lives; // Decrement count on incorrect guess
        if (Lives > 0)
        {
            PrintLine(TEXT("You have lost a life."));
            PrintLine(FString::Printf(TEXT("%i attempts remain."), --count));

            FCount Score = HitMiss(Guess);
            PrintLine(TEXT("You have %i hits and %i incorrect placements."), Score.Hits, Score.Miss);

            if (HiddenWord.Len() != Guess.Len()) // Incorrect length
            {
                PrintLine(FString::Printf(TEXT("The Hidden Word is %i characters."), HiddenWord.Len()));
                return;
            }

            // Check if the word is an isogram
            if (!IsIsogram(Guess))
            {
                PrintLine(TEXT("No repeating letters, try again."));
                return;
            }
            return;
        }
        else // Loss condition
        {
            ClearScreen();
            PrintLine(TEXT("No lives remaining!"));
            PrintLine(TEXT("The word was: %s"), *HiddenWord);
            EndGame();
            return;
        }
    }
}

/* Setup the initial game loop and introduce user */
void UBullCowCartridge::GameSetUp()
{
    HiddenWord = Isograms[FMath::RandRange(0, AuthenticateWords(Words).Num() - 1)];
    Lives = HiddenWord.Len() + 5;
    bGameLoop = true;
    // Welcome the player
    PrintLine(TEXT("Welcome to the Word Guessing Game!!"));
    PrintLine(FString::Printf(TEXT("Guess the %i letter word!"), HiddenWord.Len()));
    // Debug line below
    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord);
    PrintLine(TEXT("Type in the guess. \nPress enter to continue..."));
}

/* Ends game loop and prompts to play again */
void UBullCowCartridge::EndGame()
{
    bGameLoop = false;
    PrintLine(TEXT("\nPress enter to play again!"));
}

/* Prints out winning screen to user*/
void UBullCowCartridge::WinScreen()
{
    bGameLoop = false;
    ClearScreen();
    PrintLine(TEXT("\nCongratulations! You have won!"));
    PrintLine(TEXT("The word was: %s"), *HiddenWord);
    PrintLine(TEXT("\nPress enter to play again!"));
}

/* 
Check if the user entered in a IsoGram 
Return: True if Isogram, otherwise returns false
*/
bool UBullCowCartridge::IsIsogram(const FString &HWord) const
{
    for (int32 i = 0; i < HWord.Len(); i++) // Iterate the first index
    {
        for (int32 j = i + 1; j < HWord.Len(); j++) // Compare to next value
        {
            if (HWord[i] == HWord[j]) // If first character is the same as the next
            {
                return false;
            }
        }
    }
    return true;
}

/* 
Test the validity of the words that are imported from the text file 
Returns: Returns TArray with suitable words inside
*/
TArray<FString> UBullCowCartridge::AuthenticateWords(const TArray<FString> &WordList) const
{
    TArray<FString> Valid;
    for (FString WordTemp : WordList)
    {
        if (WordTemp.Len() >= 4 && WordTemp.Len() <= 8)
        {
            if (IsIsogram(WordTemp)) // If true
            {
                Valid.Emplace(WordTemp); // Add the word
            }
        }
    }
    return Valid; // Return the list with new words
}

/* 
Determines the number of correct characters to the hidden word that the User guessed
Returns: Type struct which contains the count of correct/incorrect
*/
FCount UBullCowCartridge::HitMiss(const FString &Guess) const
{
    FCount Count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i]) // Letter of guess is same as hidden word
        {
            Count.Hits++;
            continue;
        }
        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j]) // Right letter, wrong place
            {
                Count.Miss++;
                break;
            }
        }
    }

    return Count;
}