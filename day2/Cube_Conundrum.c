#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
    for (int i = 0; i <100; i++) {
        sum += inputParser (file, games, i);
    }


     return 0;
}

int inputParser (FILE *file, struct game games[GAMECOUNT], int game) {
    char *line[MAX];
    int currentPos = 0;
    char segment[MAX];
    char splitter = ';'; 
    memset(line, '\0', sizeof(line));
    fgets(line, sizeof(line), file);
    char *ret = line; 
    while (ret != NULL) {
        ret = memchr(line, splitter, sizeof(line));
        int pos = strlen(ret);
        memcpy(segment, line, pos);
        char *red = "red";
        char *blue = "blue";
        char *green = "green";
        int blueCount = segmentNumber (blue, segment);
        int redCount = segmentNumber (red, segment);
        int greenCount = segmentNumber (green, segment);
        if (games[game].maxRed < redCount) {
            games[game].maxRed = redCount; 
        }
        if (games[game].maxBlue < blueCount) {
            games[game].maxBlue = blueCount;
        }
        if (games[game].maxGreen < greenCount)
            games[game].maxGreen = greenCount; 
    }
    if (games[game].maxRed > MAXRED || games[game].maxBlue > MAXBLUE || games[game].maxGreen > MAXGREEN) {
        return 0;
    }
    return 1;

}



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
    char *newDigit;
    memcpy(newDigit, pos, len);
    return atoi(newDigit);
}




