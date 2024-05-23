#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define ROWS 223
#define Lenght 118
#define Numbers 25
#define Winners 10

struct slot {
int key;
int value;
int empty;
};

struct Hashtable {
    struct slot* slots;
    int UsedSlots;
};



int DeterminePoints (int numbers[Numbers], int winners[Winners]); 
int Hash (int key, int size); 
int HashtableSize (struct Hashtable *h);
void HashtableInsert(struct Hashtable *h, int key, int value);
struct Hashtable *HashTableNew ();
void HashtableFree (struct Hashtable *h);

int main(int argc, char *argv[]) {
    FILE *file;
    fopen_s(&file, "input.txt", "r");
    char line[Lenght];
    int numbers[Numbers];
    int winners[Winners];
    int game;
    int sum = 0;
    char *ptr;
    for (int i = 0; i < ROWS; i++) {
        fgets(line, Lenght*sizeof(char), file);
        ptr = strstr(line, ":");
        ptr++;
            for (int j = 0; j < Winners; j++) {
                sscanf(ptr, "%2d", &winners[j]);
                ptr = ptr+3;
            }
            ptr++;
            ptr++;
            ptr++;
            for (int j = 0; j < Numbers; j++) {
                sscanf(ptr, "%2d", &numbers[j]);
                printf("%d\n", numbers[j]);
                ptr = ptr+3;
              }
        sum += DeterminePoints(numbers, winners);
    }   
    printf("%d", sum);
    
    return EXIT_SUCCESS;
}

int DeterminePoints (int numbers[Numbers], int winners[Winners]) {
   struct Hashtable *h = HashTableNew();
 
   for (int i = 0; i < Numbers; i++) {
    HashtableInsert(h, numbers[i], numbers[i]);
   }
   for (int i = 0; i < Winners; i++) {
    HashtableInsert(h, winners[i], winners[i]);
   }
    printf("%d\n", h->UsedSlots);


   int winning = (Winners + Numbers) - h->UsedSlots;
    HashtableFree(h);
    return pow(2, winning-1);
}

struct Hashtable *HashTableNew () {
    int size = Winners + Numbers;
    struct Hashtable *hashtable = malloc(sizeof(*hashtable));
    hashtable->slots = malloc(size*sizeof(struct slot));
    for (int i = 0; i < size; i++) {
        hashtable->slots[i].empty = 1; 
    }
    hashtable->UsedSlots = 0;
    return hashtable;
}

void HashtableInsert(struct Hashtable *h, int key, int value) {
    int size = Winners + Numbers;
    int index = Hash(key, (Winners + Numbers));
    while (h->slots[index].empty == 0 && h->slots[index].key != key) {
        index++;
        index = index % size;
        printf("index: %d \n", index);
    }
    h->slots[index].key = key;
    h->slots[index].value = value;
    if (h->slots[index].empty == 1) {
        h->slots[index].empty = 0;
        h->UsedSlots++;
    }

    return;
}

int Hash (int key, int size) {
    return key % size;
}

int HashtableSize (struct Hashtable *h) {
    return h->UsedSlots;
}

void HashtableFree (struct Hashtable *h) {
    free(h->slots);
    free(h);
}





