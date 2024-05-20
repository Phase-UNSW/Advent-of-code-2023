#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 512
#define MAXRED 12
#define MAXGREEN 13
#define MAXBLUE 14
int inputParser(char *line, int i); 
int colorCounter (char *colour, char *segment);



int main(int argc, char *argv[])
{
    FILE *file;
    int error_code = fopen_s(&file, "input.txt", "r");
    if (error_code != 0) {
        printf("Error");
        return 1;
    }
    char *line = malloc(sizeof(char)*MAX);
    int sum = 0;
    for (int i = 1; i <= 100; i++) {
        printf("%d\n", i);
        fgets(line, MAX, file);
        sum += inputParser(line, i);
    }
    printf("%d", sum);
    
    return EXIT_SUCCESS;
}

int inputParser(char *line, int i) {
    int blueCount;
    int redCount;
    int greenCount;
    char splitter = ';';
    char* end = line;
    int total;
    char segment[MAX];
    int counter = 0;
    int seg_len;
    while (end != NULL) {
        total = strlen(end);
        end = strchr(line, splitter);
        if (end != NULL) {
            seg_len = total - strlen(end);
        } else {
            seg_len = total;
        } 
        memcpy(segment, line, seg_len*sizeof(char));
        segment[seg_len] = '\0';

        redCount = colorCounter("red", segment);
        greenCount = colorCounter("green", segment);
        blueCount = colorCounter("blue", segment);

        if (blueCount > MAXBLUE || redCount > MAXRED || greenCount > MAXGREEN) {
            return 0;
        }
        if (end != NULL) {
            end++;
        }
        line = end;
    }
   return i; 

}

int colorCounter (char *colour, char *segment) {
    char *pos = strstr(segment, colour);
    if (pos == NULL) {
        return 0;
    }
    int len = 0;
    pos -= 2;
    while (isdigit(pos[0])) {
        len++;
        pos--;
    }
    pos++;
    char *digit = malloc(sizeof(char)*(len+1));
    memcpy(digit, pos, len*sizeof(char));
    digit[len] = '\0';
    int result = atoi(digit);
    free(digit);
    return result;
}
