#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX 256
#define GAMECOUNT 100
#define TRUE 1
#define MAXGREEN 13
#define MAXRED 12
#define MAXBLUE 14

struct game {
    int maxRed;
    int maxGreen;
    int maxBlue; 
};
int segmentNumber (char* colour, char* segment);
int inputParser (FILE *file, struct game games[GAMECOUNT], int game); 
int main(int argc, char const *argv[])
{
    FILE *file;
    int sum = 0;
    file = fopen("input.txt", "r");
    if (file == NULL)
    {
        perror("File is invalid");
        return 1;
    }
        
    struct game games[GAMECOUNT];
    for (int i = 0; i < 100; i++) {
        printf("%d", i);
        games[i].maxRed = 0;
        games[i].maxBlue = 0;
        games[i].maxGreen = 0;
        sum += inputParser (file, games, i);
    }

    printf("%d", sum);
    fclose(file);
    return 0;
}

int inputParser (FILE *file, struct game games[GAMECOUNT], int game) {
       char line[MAX];
    char segment[MAX];
    char splitter = ';';
    if (fgets(line, MAX, file) == NULL) {
        return 0;
    }
    
    char *currentPos = line;
    char *ret = NULL;
    int blueCount;
    int greenCount;
    int redCount;
    const char *red = "red";
    const char *blue = "blue";
    const char *green = "green";

    while ((ret = strchr(currentPos, splitter)) != NULL) {
        int pos = ret - currentPos;
        memcpy(segment, currentPos, pos * sizeof(char));
        segment[pos] = '\0';  // Null-terminate the segment

        blueCount = segmentNumber(blue, segment);
        redCount = segmentNumber(red, segment);
        greenCount = segmentNumber(green, segment);

        if (games[game].maxRed < redCount) {
            games[game].maxRed = redCount; 
        }
        if (games[game].maxBlue < blueCount) {
            games[game].maxBlue = blueCount;
        }
        if (games[game].maxGreen < greenCount) {
            games[game].maxGreen = greenCount; 
        }
        currentPos = ret + 1;
    }
    // Handle the last segment after the last splitter
    if (*currentPos != '\0') {
        strcpy(segment, currentPos);
        segmentNumber(blue, segment);
        blueCount = segmentNumber(blue, segment);
        redCount = segmentNumber(red, segment);
        greenCount = segmentNumber(green, segment);

        if (games[game].maxRed < redCount) {
            games[game].maxRed = redCount; 
        }
        if (games[game].maxBlue < blueCount) {
            games[game].maxBlue = blueCount;
        }
        if (games[game].maxGreen < greenCount) {
            games[game].maxGreen = greenCount; 
        }
    }
    if (games[game].maxRed > MAXRED || games[game].maxBlue > MAXBLUE || games[game].maxGreen > MAXGREEN) {
        return 0;
    }
    return game;}

int segmentNumber (char* colour, char* segment) {
    char* pos = strstr(segment, colour);
    if (pos == NULL) {
        return 0;
    }

    pos = pos - 2;
    int len = 0;
    while (isdigit(pos[0])) {
        pos--;
        len++;
    }

    pos++;
    char *newDigit = malloc(sizeof(char)*len);
    memcpy(newDigit, pos, len*sizeof(char));
    newDigit[len] = '\0';
    printf("%s %s\n", newDigit, colour);
    int result = atoi(newDigit);
    free(newDigit);
    printf("%d\n", result);

    return result;
}




