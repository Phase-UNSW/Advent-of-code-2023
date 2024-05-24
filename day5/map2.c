

#include <corecrt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define MAXCOL 209
#define SEEDS 20
struct map {
    long long start;
    long long end;
};
long long *map(FILE *file, long long* seeds); 
long long* parseSeeds(FILE *file);
int main(int argc, char *argv[])
{
    FILE *file;
    errno_t err = fopen_s(&file, "input.txt", "r");
    if (err != 0) {
        printf_s("Error :(");
    }
    long long *seeds = parseSeeds(file);
    for (int i = 0; i < 7; i++) {
        seeds = map(file, seeds);
    }

    long long min = seeds[0];
    for (int i = 0; i < SEEDS; i++) {
        if (min > seeds[i]) {
            min = seeds[i];
        }
    }

    printf("min: %lld", min);


    
    return EXIT_SUCCESS;
}

long long* parseSeeds(FILE *file) {
    char line[MAXCOL];
    fgets(line, MAXCOL, file);
    char *ptr;
    long long* seeds = NULL;
    ptr = strtok(line, " ");
    ptr = strtok(NULL, " ");
    int i = 0;
    while (ptr != NULL) {
        seeds = realloc(seeds, (i+1)*sizeof(long long));
        seeds[i] = atoll(ptr); 
        i++;
        ptr = strtok(NULL, " ");
    }
    return seeds;
}

long long *map(FILE *file, long long* seeds) {
    char line[MAXCOL];
    char *ptr = NULL;
    while (ptr == NULL) {
        fgets(line, MAXCOL, file);
        ptr = strstr(line, ":");
    }
    int read = 3;
    int row = 0;
    struct map *prev = NULL;
    struct map *next = NULL;
    long long range;
    char* ptrr = line;
    while (ptrr != NULL && read == 3) {
        prev = realloc(prev, (row+1)*sizeof(struct map));
        next = realloc(next, (row+1)*sizeof(struct map));
        ptrr = fgets(line, MAXCOL, file);
        read = sscanf_s(line, "%lld %lld %lld", &next[row].start, &prev[row].start, &range);
        next[row].end = next[row].start + range;
        prev[row].end = prev[row].start + range;
        row++;
    }
    
    for (int i = 0; i < SEEDS; i++) {
        for (int j = 0;j < row ; j++) {
            if (seeds[i] >= prev[j].start && seeds[i] <= prev[j].end) {
                seeds[i] = seeds[i] - prev[j].start + next[j].start;
                break;
            }
        }
    }
free(prev);
free(next);
return seeds;
}

