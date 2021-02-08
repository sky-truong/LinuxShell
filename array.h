#define REV_OFF 0
#define MATCH 0

/**
 * Author: Pham (Sky) Truong
 * Last revision date: Feb 8th, 2021
 * References: Personal assignment 2 from CIS2520
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Array{
    unsigned int width;
    unsigned int nel;
    unsigned int capacity;
    int index;
    int *orders;
    char **commands;
    void *data;
};

struct Array *newArray(unsigned int width, unsigned int capacity);
void writeItem(struct Array *array, unsigned int index, void *src);
void readItem(struct Array *array, unsigned int index, void *dest);
void contract(struct Array *array);
void freeArray(struct Array *array);
void appendItem(struct Array *array, int order, char *command, void *src);
void deleteItem(struct Array *array, unsigned int index);

