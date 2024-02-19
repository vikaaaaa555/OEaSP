#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <locale.h>
#include "morse.h"

int main(int argc, char* argv[]) {
    setlocale(0, "");
    char input[100];
    int choice;

    printf("Translate to or from Morse code (1/2): ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
    case 1:
        printf("Enter the text to translate into Morse code: ");
        fgets(input, sizeof(input), stdin);
        convert_to_morse(input);
        break;
    case 2:
        printf("Enter the text to translate from Morse code: ");
        fgets(input, sizeof(input), stdin);
        convert_from_morse(input);
        break;
    default:
        printf("Incorrect input. Choose 1 or 2.");
        break;
    }

    return 0;
}
