#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "morse.h"

typedef struct {
    const char* letter;
    const char* morse;
} MorseMap;

const MorseMap morse_dict[] = {
    {"A", ".-"},
    {"B", "-..."},
    {"C", "-.-."},
    {"D", "-.."},
    {"E", "."},
    {"F", "..-."},
    {"G", "--."},
    {"H", "...."},
    {"I", ".."},
    {"J", ".---"},
    {"K", "-.-"},
    {"L", ".-.."},
    {"M", "--"},
    {"N", "-."},
    {"O", "---"},
    {"P", ".--."},
    {"Q", "--.-"},
    {"R", ".-."},
    {"S", "..."},
    {"T", "-"},
    {"U", "..-"},
    {"V", "...-"},
    {"W", ".--"},
    {"X", "-..-"},
    {"Y", "-.--"},
    {"Z", "--.."}
};

void convert_to_morse(const char* input) {
    printf("The result in Morse code: ");
    for (int i = 0; i < strlen(input); i++) {
        char c = toupper(input[i]);
        if (c >= 'A' && c <= 'Z') {
            int found = 0;
            for (int j = 0; j < sizeof(morse_dict) / sizeof(MorseMap); j++) {
                if (morse_dict[j].letter[0] == c) {
                    printf("%s ", morse_dict[j].morse);
                    found = 1;
                    break;
                }
            }
            if (!found)
                printf("%c ", c);
        }
        else if (c == ' ')
            printf(" ");

        else
            printf("%c ", c);
    }
    printf("\n");
}



void convert_from_morse(const char* input) {
    printf("Morse code translation: ");
    char* token = strtok((char*)input, " \t\n\r");
    while (token != NULL) {
        int found = 0;
        for (int i = 0; i < sizeof(morse_dict) / sizeof(MorseMap); i++) {
            if (strcmp(token, morse_dict[i].morse) == 0) {
                printf("%s", morse_dict[i].letter);
                found = 1;
                break;
            }
        }
        if (!found)
            printf("This isn\'t Morse code.");

        token = strtok(NULL, " \t\n\r");
    }
    printf('\n');
}

