#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX 256
void arrayIntialiser (char line[]);
int main(int argc, char const *argv[])
{
    FILE *file;
    file = fopen("input.txt", "r");

    if (file == NULL) {
        perror("File is invalid");
        return 1;
    }
    char line[256] = {};
    arrayIntialiser(line);
    int sum = 0;
    char string[3] = {"\0"};
    string[0] = 'a';
    string[1] = 'b';
    string[3] = '\0';

    while(fgets(line, sizeof(line), file) != NULL) {
        for (int i = 0; i < (sizeof(line)/sizeof(char)); i++)
        {
            if (isdigit((line[i])) && !isdigit(string[0])) {
                string[0] = line[i];

            }
            else if (isdigit(line[i])) {
                string[1] = line[i];
            }   
        }
        if (string[1] == 'b') {
            string[1] = string[0];
        }
        printf("%s\n", string);
        sum += atoi(string);
        string[0] = 'a';
        string[1] = 'b' ;
        
        arrayIntialiser(line);
    }
    printf("%d\n", sum);
    return 0;
}

void arrayIntialiser (char line[MAX]) {
    for (int i = 0; i < MAX; i++)
    {
        line[i] = '\0';
    }
    return;
}
