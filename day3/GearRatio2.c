#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define ROWS 140
#define COLUMNS 141
#define EMPTY -1
void determineGearRatio (char **array, int row, int col, int *sum);

struct gear {
    int row;
    int col;
    int number;
};
void findDigits (int col, int row, struct gear gears[2], char** array); 
int main(int argc, char *argv[])
{
    FILE *file;
    fopen_s(&file, "input.txt", "r");
    int sum = 0;
    char **array = malloc((ROWS)*sizeof(*array)); 
    for (int i = 0; i < ROWS; i++) {
        array[i] = malloc(sizeof(array[0]) * COLUMNS);
    }
    for (int i = 0; i < ROWS; i++) {
        fgets(array[i], (COLUMNS+1)*sizeof(char), file);
        array[i][COLUMNS] = '\0';
    }
    fclose(file);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
           determineGearRatio(array, row, col, &sum);
        }
    }

    printf("%d", sum);
    return EXIT_SUCCESS;
}

void determineGearRatio (char **array, int row, int col, int* sum) {
   if (array[row][col] != '*') {
        return;
   }
    int current_col;
    int current_row;
    struct gear gears[2];
    gears[0].col = EMPTY;
    gears[1].col = EMPTY;
    gears[0].row = EMPTY;
    gears[1].row = EMPTY;
 
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            current_col = col + i;
            current_row = row + j;
            findDigits(current_col, current_row, gears, array); 
        }
    }
    if (gears[0].col != EMPTY && gears[1].col != EMPTY) {
        printf("%d, %d\n", gears[0].number, gears[1].number);
        (*sum) += gears[0].number * gears[1].number; 
        
    }    
    return;
}


void findDigits (int col, int row, struct gear gears[2], char **array) {
    if (!isdigit(array[row][col])) {
        return;
    }
    //printf ("%d, %d ", row, col);
    int start = col;
    while (start >= 0 && isdigit(array[row][start])) {
        start--;
    }
    start++;
    int end = col; 
    while (end < COLUMNS && isdigit(array[row][end])) {
        end++;
    }
    if (gears[0].col == start && gears[0].row == row) {
        return;
    }
    int len = end - start;
    char *line = malloc((len+1)*sizeof(line));
    for (int i = 0; i < len; i++) {
        line[i] = array[row][start+i];
    }
    line[len] = '\0';
    int number = atoi(line);
    if (gears[0].col == EMPTY) {
        gears[0].col = start;
        gears[0].row = row;
        gears[0].number = number;
    }
    else {
        gears[1].col = start;
        gears[1].row = row;
        gears[1].number = number;
    }
}

