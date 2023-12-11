#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100

int WordToNumber (char line[MAX], int position);
int main(int argc, char const *argv[])
{
    FILE *file;
    file = fopen("input.txt", "r");

    if (file == NULL) {
        perror("File is invalid");
        return 1;
    }
    char line[MAX] = {};
    char string[3];
    string[0] = 'a';
    string[1] = 'b';
    int sum = 0;
    memset(line, '\0', sizeof(line));
    while (fgets(line, sizeof(line), file) != NULL) {
        for (int i = 0; i < (sizeof(line)/sizeof(char)); i++) {
            if (isdigit((line[i])) && (!isdigit(string[0]))) {
                string[0] = line[i];
            }
            else if (WordToNumber(line, i) != -1 && !isdigit(string[0])) {
                string[0] = WordToNumber(line, i) + '0';
            }
            else if (isdigit(line[i])) {
                string[1] = line[i];
            }
            else if (WordToNumber(line, i) != -1) {
                string[1] = WordToNumber(line, i) + '0';
            }
        }
        if (string[1] == 'b') {
            string[1] = string[0];
        }
        printf("%s\n", string);
        sum += atoi(string);
        string[0] = 'a';
        string[1] = 'b';
        memset(line, '\0', sizeof(line));
        }
        printf("%d", sum);
    return 0;
}

int WordToNumber (char line[MAX], int position) {
    int string_lenght = strlen(line);
    char three_digit_number[4];
    three_digit_number[3] = '\0';
    if (string_lenght - position >= 3) {
        strncpy(three_digit_number, &line[position], 3);
            if (strncmp(three_digit_number, "one", 3) == 0) {
            return 1;
        }
        else if (strncmp(three_digit_number, "two", 3) == 0) {
            return 2;
        }
        else if (strncmp(three_digit_number, "six", 3) == 0) {
            return 6;
        }
        
    }
    char four_digit_number[5];
    four_digit_number[4] = '\0';
    if (string_lenght - position >= 4) {
        strncpy(four_digit_number, &line[position], 4);
        if (strncmp(four_digit_number, "four", 4) == 0) {
            return 4;
        }
        else if (strncmp(four_digit_number, "five", 4) == 0) {
            return 5;
        }
        else if (strncmp(four_digit_number, "zero", 4) == 0) {
            return 0;
        }
        else if (strncmp(four_digit_number, "nine", 4) == 0) {
            return 9;
        }        
    }
    char five_digit_number[6];
    five_digit_number[5] = '\0';
    if (string_lenght - position >= 5) {
        strncpy(five_digit_number, &line[position], 5);
        if (strncmp(five_digit_number, "three", 5) == 0) {
            return 3;
        }
        else if (strncmp(five_digit_number, "seven", 5) == 0) {
            return 7;
        }
        else if (strncmp(five_digit_number, "eight", 4) == 0) {
            return 8;
        }
    }
    return -1;
}
