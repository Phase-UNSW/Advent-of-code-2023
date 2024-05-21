#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ROWS 140
#define COLUMNS 141
int determineSerialNumber (char **array, int row, int col, int *sum);


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
           col = determineSerialNumber(array, row, col, &sum);
        }
    }

    printf("%d", sum);
    return EXIT_SUCCESS;
}

int determineSerialNumber (char **array, int row, int col, int* sum) {
   if (!isdigit(array[row][col])) {
        return col;
   }
   
    int len = 1;
    int temp_col = col;
    while (temp_col < COLUMNS && isdigit(array[row][temp_col])) {
        temp_col++;
        len++;

    }
    char *line = malloc((len+1)*sizeof(line));
    for (int i = 0; i < len; i++) {
        line[i] = array[row][col+i];
    }
    line[len] = '\0';
    for (int i = row - 1; i < ROWS && i < row + 2 ; i++) {
        if (i < 0) {
            continue;
        }
        for (int j = col - 1; j < (COLUMNS-1) && j < col + len; j++) {
            if (j < 0) {
                continue;
            }
            if (array[i][j] != '.' && !isdigit(array[i][j]) && array[i][j] != '\n') {
                (*sum) += atoi(line);
            }            
            
        }
    }
    return temp_col;

}
