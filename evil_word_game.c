#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Pattern_struct {
    char* pat;  //string pattern - exs: "hello", "-ell-", "-----"
    int count;  //frequency of pattern in the current word list
    int changes;//number of occurences of new letter in this pattern
} Pattern;

//adds words to the word list
void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    if(*numWords != *maxWords) {
        (*words)[*numWords] = (char*)malloc((strlen(newWord)+1) * sizeof(char));
        strcpy((*words)[*numWords], newWord);
    }
    else {
        *maxWords *= 2;
        char** newList = (char**)malloc(*maxWords * sizeof(char*));
        
         for(int i = 0; i < *numWords; i++) {
             newList[i] = (char*)malloc((strlen((*words)[i]) + 1) * sizeof(char));
             strcpy(newList[i], (*words)[i]);
         }
         newList[*numWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char));
         strcpy(newList[*numWords], newWord);
         for (int i = 0; i < *numWords; i++) {
            free((*words)[i]);
        }
        free(*words);
        *words = newList;
    }
    (*numWords)++;
}
//prints out if a mode is on or off
void boolConv(bool item) {
    if(item) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}

// returns count of character differences between two strings;
// includes extra characters in longer string if different lengths
int strNumMods(char* str1, char* str2) {
    int difCounter = abs(strlen(str1) - strlen(str2));
    int smallLen;
    if(strlen(str1) > strlen(str2)) {
        smallLen = strlen(str2);
    }
    else {
        smallLen = strlen(str1);
    }
    for(int i = 0; i < smallLen; i++) {
        if(str1[i] != str2[i]) {
            difCounter++;
        }
    }
    return difCounter;
}

// returns index of the character where two strings first differ, &
// returns strlen(str1) = strlen(str2) if none
int strDiffInd(char* str1, char* str2) {
    for(int i = 0; i < strlen(str1); i++) {
        if(str1[i] != str2[i]) {
            return i;
        }
    }
    return strlen(str1);
}
//removes words when the pattern is not updated
void removeWords(char*** words, int* numWords, char letter) {
    int newNumWords = *numWords;
    char** newList = (char**)malloc(*numWords * sizeof(char*));
    int counter = 0;
    for(int i = 0; i < *numWords; i++) {
        if(strchr((*words)[i], letter) == NULL) {
            newList[counter] = (char*)malloc((strlen((*words)[i]) + 1) * sizeof(char));
            strcpy(newList[counter], (*words)[i]);
            counter++;
        }
        else {
            newNumWords--;
        }
    }
    for(int i = 0; i < *numWords; i++) {
        free((*words)[i]);
    }
    free(*words);
    (*words) = newList;
    (*numWords) = newNumWords; 
}
//removes words when pattern is updated
void removeWordV2(char*** words, int* numWords, char *pattern, int wordSize, char letter) {
    int numLetters = 0;
    int numLet = 0;
    for(int i = 0; i < wordSize; i++) {
        if(isalpha(pattern[i]) == 0) {
                numLetters++;
        }
        if(pattern[i] == letter) {
            numLet++;
        }
    }
    int newNumWords = *numWords;
    char** newList = (char**)malloc(*numWords * sizeof(char*));
    int counter = 0;
    for(int i = 0; i < *numWords; i++) {
        int curLets = 0;
        for(int x = 0; x < wordSize;x++) {
            if((*words)[i][x] == letter) {
                curLets++;
            }
        }
        if(strNumMods((*words)[i], pattern) == numLetters && curLets == numLet) {
            newList[counter] = (char*)malloc(wordSize * sizeof(char));
            strcpy(newList[counter], (*words)[i]);
            counter++;
        }
        else {
            newNumWords--;
        }
    }
    for(int i = 0; i < *numWords; i++) {
        free((*words)[i]);
    }
    free(*words);
    (*words) = newList;
    (*numWords) = newNumWords; 
}
//checks if a pattern niss in the list and if it is updates the count
bool patternAdder(Pattern** patternList, char* pattern, int* numPat) {
    for(int i = 0; i < *numPat; i++) {
        if(strcmp((*patternList)[i].pat, pattern) == 0) {
            (*patternList)[i].count++;
            return  true;
        }
    }
    return false;
}
//builds the pattern list
void patternFormer(char*** words, Pattern** patternList, int numWords, int* maxPat, int* numPat, char letter, int wordSize, char *finalPat) {
    char let[2];
    let[0] = letter;
    let[1] = '\0';
    //build the pattern for each word
    for(int i = 0; i < numWords; i++) {
        char curPat[wordSize + 1];
        curPat[0] = '\0';
        for(int curLet = 0; curLet < wordSize; curLet++) {
            char patLet[2];
            patLet[0] = finalPat[curLet];
            patLet[1] = '\0';
            if(finalPat[curLet] != '-') {
                strcat(curPat, patLet);
            }
            else if((*words)[i][curLet] != letter) {
                strcat(curPat, "-");
            }
            else {
                strcat(curPat, let);
            }
        }
        //checks if current pattern is already in pattern list
        //if its not add it to the list and if the list needs more space double its size
        if(!patternAdder(patternList, curPat, numPat)) {
            if(*numPat < *maxPat) {
                (*patternList)[*numPat].pat = (char*)malloc(wordSize * sizeof(char));
                strcpy((*patternList)[*numPat].pat, curPat);
                (*patternList)[*numPat].count = 1;
                (*patternList)[*numPat].changes = strNumMods(finalPat, curPat);
            }
            else {
                *maxPat *= 2;
                Pattern* newList = (Pattern*)malloc(*maxPat * sizeof(Pattern));
                
                for(int i = 0; i < *numPat; i++) {
                    newList[i].pat = (char*)malloc((wordSize + 1) * sizeof(char));
                    strcpy(newList[i].pat, ((*patternList)[i].pat));
                    newList[i].count = (*patternList)[i].count;
                    newList[i].changes = (*patternList)[i].changes;
                    
                }
                newList[*numPat].pat = (char*)malloc((wordSize + 1) * sizeof(char));
                strcpy(newList[*numPat].pat, curPat);
                newList[*numPat].count = 1;
                newList[*numPat].changes = strNumMods(finalPat, curPat);
                for (int i = 0; i < *numPat; i++) {
                    free((*patternList)[i].pat);
                }
                free(*patternList);
                *patternList = newList;
            }
            (*numPat)++;
        }
        
    }
    //finds the pattern with the highest count
    int highestCount = 0;
        int index = 0;
        for(int i = 0; i < *numPat; i++) {
            if((*patternList)[i].count > highestCount) {
                highestCount = (*patternList)[i].count;
                index = i;
            }
            //if counts equal then check changes
            else if((*patternList)[i].count == highestCount) {
                if((*patternList)[i].changes < (*patternList)[index].changes) {
                    highestCount = (*patternList)[i].count;
                    index = i;
                }
                //if changes are equal finds the pattern with the first instance of a letter
                else if((*patternList)[i].changes == (*patternList)[index].changes) {
                    int x = 0;
                    bool found = false;
                    while(!found && x != wordSize) {
                        if((*patternList)[i].pat[x] != (*patternList)[index].pat[x]) {
                            if(isalpha((*patternList)[i].pat[x])) {
                                highestCount = (*patternList)[i].count;
                                index = i; 
                            }
                            found = true;
                        }
                        x++;
                    }

                }
            }
        }
    strcpy(finalPat, (*patternList)[index].pat);
}

int main(int argc, char* argv[]) {
    printf("Welcome to the (Evil) Word Guessing Game!\n\n");
    bool solved = false;
    int wordSize = 5;
    int numGuesses = 26;
    bool statsMode = false;
    bool wordListMode = false;
    bool letterListMode = false;
    bool patternListMode = false;
    bool extensionMode = false;
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i],"-n") == 0) {
            if(atoi(argv[i + 1]) > 1 && atoi(argv[i + 1]) < 30) {
                wordSize = atoi(argv[i + 1]);
                i++;
            }
            else{
                printf("Invalid word size.\nTerminating program...\n");
                return -1;
            }
        }
        else if(strcmp(argv[i],"-g") == 0) {
            if(atoi(argv[i + 1]) > 0) {
                numGuesses = atoi(argv[i + 1]);
            }
            else {
                printf("Invalid number of guesses.\nTerminating program...\n");
                return 1;
            }
            i++;
        }
        else if(strcmp(argv[i],"-s") == 0) {
            statsMode = true;
        }
        else if(strcmp(argv[i],"-w") == 0) {
            wordListMode = true;
        }
        else if(strcmp(argv[i],"-l") == 0) {
            letterListMode = true;
        }
        else if(strcmp(argv[i],"-p") == 0) {
            patternListMode = true;
        }
        else if(strcmp(argv[i],"-v") == 0) {
            statsMode = true;
            wordListMode = true;
            letterListMode = true;
            patternListMode = true;
        }
        else {
            printf("Invalid command-line argument.\nTerminating program...\n");
            return 1;
        }
    }
    printf("Game Settings:\n");
    printf("  Word Size = %d\n", wordSize);
    printf("  Number of Guesses = %d\n", numGuesses);
    printf("  View Stats Mode = ");
    boolConv(statsMode);
    printf("  View Word List Mode = ");
    boolConv(wordListMode);
    printf("  View Letter List Mode = ");
    boolConv(letterListMode);
    printf("  View Pattern List Mode = ");
    boolConv(patternListMode);
    
    int capacity = 4;
    int numWords = 0;
    int totalCount = 0;
    char longestWord[50];
    char** wordList = (char**)malloc(capacity*sizeof(char*));

    FILE *inFile = NULL;
    char fileWord[50];
    int numRead;
    inFile = fopen("dictionary.txt", "r");
    while(fgets(fileWord, 50, inFile)) {
        fileWord[strcspn(fileWord, "\n")] = '\0';
        totalCount++;
        if(strlen(fileWord) == wordSize) {
            addWord(&wordList, &numWords, &capacity, fileWord);
        }
        if(strlen(fileWord) > strlen(longestWord)) {
            strncpy(longestWord, fileWord, strlen(fileWord));
        }
    }
    fclose(inFile);
    if(statsMode == true) {
        printf("The dictionary contains %d words total.\n", totalCount);
        printf("The longest word %s has %ld chars.\n", longestWord, strlen(longestWord));
        printf("The dictionary contains %d words of length %d.\n", numWords, wordSize);
        printf("Max size of the dynamic words array is %d.\n", capacity);

    }
    if(wordListMode == true) {
        printf("Words of length %d:\n", wordSize);
        if(numWords > 0) {
            for(int i = 0; i < numWords; i++) {
                printf("  %s\n", wordList[i]);
            }
        }
        else {
            printf("Dictionary has no words of length %d.\nTerminating program...\n", wordSize);
            return 1;
        }
    }
    //main game starts here
    char userInput;
    char finalWord[wordSize + 1];
    memset(finalWord, '-', wordSize);
    finalWord[wordSize] = '\0';
    printf("The word pattern is: %s\n\n", finalWord);
    char guessedLetters[numGuesses];
    int guesses = 0;
    while(!solved && numGuesses > 0) {
        char lastPat[wordSize + 1];
        strcpy(lastPat, finalWord);
        int numPat = 0;
        int maxPat = 4;
        bool valid = true;
        bool alreadyGuessed = false;
        Pattern* patternList = (Pattern*)malloc(maxPat*sizeof(Pattern));

        printf("Number of guesses remaining: %d\n", numGuesses);
        if(letterListMode) {
            printf("Previously guessed letters: ");
            for(int i = 0; i < guesses; i++) {
                printf("%c ", guessedLetters[i]);
            }
            printf("\n");
        } 
        printf("Guess a letter (# to end game): ");
        scanf(" %c", &userInput);
        for(int i = 0; i < numGuesses; i++) {
            if(guessedLetters[i] == userInput) {
                alreadyGuessed = true;
            }
        }
        if(userInput == '#') {
            printf("Terminating game...\n");
            return 1;
        }
        else if (alreadyGuessed) {
            printf("Letter previously guessed...\n");
        }
        else if (!isalpha(userInput) || !islower(userInput)) {
            printf("Invalid letter...\n");
            valid = false;
        }
        else {
            //puts the guessed letters in an array
            guessedLetters[guesses] = userInput;
            guesses++;
            patternFormer(&wordList, &patternList, numWords, &maxPat, &numPat, userInput, wordSize, finalWord);
            
            if(patternListMode) {
                printf("All patterns for letter %c: \n", userInput);
                for(int i = 0; i < numPat; i++) {
                    printf("  %s \tcount = %d \tchanges = %d\n", patternList[i].pat, patternList[i].count, patternList[i].changes);
                }
            }
            if(strcmp(finalWord, lastPat) == 0) {
                printf("\nOops, there are no %c's. You used a guess.\n", userInput);
                removeWords(&wordList, &numWords, userInput);
                numGuesses--;
                strcpy(finalWord, lastPat);
            }
            else {
                printf("\nGood guess! The word has at least one %c.\n", userInput);
                removeWordV2(&wordList, &numWords, finalWord, wordSize, userInput);
            }

            if(wordListMode) {
                printf("Possible words are now:\n");
                for(int i = 0; i < numWords; i++) {
                    printf("  %s\n", wordList[i]);
                }
            }
            int checker = 0;
            for(int i = 0; i < wordSize; i++) {
                if(isalpha(finalWord[i])) {
                    checker++;
                }
            }
            if(checker == wordSize) {
                solved = true;
            }
            for(int i = 0; i < numPat; i++) {
                free(patternList[i].pat);
            }
            free(patternList);  
        }
        if(statsMode) {
            printf("Number of possible words remaining: %d\n", numWords);
        }
        if(valid) {
            printf("The word pattern is: %s\n\n", finalWord);
        }
    }
    if(solved) {
        printf("You solved the word!\n");
    }
    else {
        printf("You ran out of guesses and did not solve the word.\n");
        char answer[wordSize];
        strcpy(answer, wordList[0]);
        for(int i = 1; i < numWords; i++) {
            if(strcmp(answer, wordList[i]) > 0) {
                strcpy(answer, wordList[i]);
            }
        }
        strcpy(finalWord, answer);
    }
    printf("The word is: %s\nGame over.\n", finalWord);

    for(int i = 0; i < numWords; i++) {
        free(wordList[i]);
    }
    free(wordList);
    return 0;
}
