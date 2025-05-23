#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <windows.h> // Incluído para usar Sleep

void glitch_text(char *text, int glitch_amount) {
    int len = strlen(text);
    srand(time(0));

    for (int i = 0; i < glitch_amount; i++) {
        int index = rand() % len;
        char random_char = '!' + (rand() % (126 - '!' + 1));
        text[index] = random_char;
    }
}

void print_with_glitch(char *text, int glitch_amount, int delay) {
    int len = strlen(text);
    char *temp = (char*) malloc(len + 1); 

    for (int i = 0; i < len; i++) {
        strncpy(temp, text, i + 1);
        temp[i + 1] = '\0';

        for (int j = 0; j < glitch_amount; j++) {
            glitch_text(temp, i + 1);
            printf("\r%s", temp);
            fflush(stdout);
            Sleep(delay); // Substituído usleep por Sleep
        }

        temp[i] = text[i]; // Substitua o caractere glitchado pela letra original
        printf("\r%s", temp);
        fflush(stdout);
        Sleep(delay); // Substituído usleep por Sleep
    }

    free(temp);
}

int main() {
	setlocale(LC_ALL, "");
    char text[] = "Me da minha folha";
    print_with_glitch(text, 150, 10);
    /* system("cls");
    printf("%s", text); */

    return 0;
}
