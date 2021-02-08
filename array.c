/**
 * Author: Pham (Sky) Truong
 * Last revision date: Feb 8th, 2021
 * References: Personal assignment 2 from CIS2520
 */ 

#include "array.h"

struct Array *newArray(unsigned int width, unsigned int capacity){
    struct Array *newArr = malloc(sizeof(struct Array));
    if(newArr == NULL){
        fprintf(stderr, "Error: out of buffer. Exiting...\n");
        exit(-1);
    }
    newArr->width = width;
    newArr->capacity = capacity;
    newArr->nel = 0;
    newArr->index = 0;
    newArr->commands = malloc(sizeof(char*)*capacity);
    newArr->orders = malloc(sizeof(int*)*capacity);
    newArr->data = malloc(sizeof(void*)*(width*capacity));
    if(newArr->data == NULL){
        fprintf(stderr, "Error: out of buffer. Exiting...\n");
        exit(-1);
    }
    return newArr;
}

void getbytes(unsigned char dest[], int bytes, void *src, int reverse){

  unsigned char *csrc = src;
  unsigned char *cdest = dest;
  int i;
  int j = bytes-1;

  if(reverse == 0 ){
    for(i = 0; i < bytes; i++){
      cdest[i] = csrc[i];
    }
  }else{
    for(i = 0; i < bytes; i++){
      cdest[i] = csrc[j];
      j--;
    }
  }
}

void writeItem(struct Array *array, unsigned int index, void *src){
    if(index > array->nel || index >= array->capacity){
        fprintf(stderr, "Error: Writing out of bounds. Exiting...\n");
        exit(-1);
    }else if(index == array->nel){
        array->nel++;
    }

    unsigned int offset = index * (array->width); //starting byte
    unsigned char *temp = array->data; 
    unsigned char *dest = temp + offset; //place pointer at byte
    unsigned char *csrc = src;
    getbytes(dest, array->width, csrc, REV_OFF);
    //printf("%d\n", *dest);
}

void readItem(struct Array *array, unsigned int index, void *dest){
    if(index >= array->nel){
        fprintf(stderr, "Error: Reading out of bounds. Exiting...\n");
        exit(-1);
    }

    unsigned int offset = index * (array->width); //starting byte
    unsigned char *temp = array->data; 
    unsigned char *csrc = temp + offset; //place pointer at byte
    unsigned char *cdest = dest;
    getbytes(cdest, array->width, csrc, REV_OFF);
    //printf("%d\n", *cdest);
}

void contract(struct Array *array){
    if(array->nel == 0){
        fprintf(stderr, "Error: already empty. Exiting...\n");
    }
    array->nel--;
}

void freeArray(struct Array *array){
    for(int i = 0; i < array->index; i++) {
        free(array->commands[i]);
    }
    free(array->commands);
    free(array->orders);
    free(array->data);
    free(array);
}

void appendItem(struct Array *array, int order, char *command, void *src){
    int i = array->index;
    array->orders[i] = order;
    array->commands[i] = malloc(sizeof(char)*256);
    strcpy(array->commands[i],command);
    writeItem(array, array->nel, src);
    array->index++;
}

void deleteItem(struct Array *array, unsigned int index){
    unsigned int offset = (index+1) * (array->width); //the byte after the item to be deleted
    unsigned char *temp = array->data;
    unsigned char *src = temp + offset; //point to that byte

    while(index < array->nel-1){
        readItem(array, index+1, src);
        writeItem(array, index, src);
        index++;
        offset = index * (array->width); //the next byte to be copied forward
        temp = array->data;
        src = temp + offset; //point to that byte
    }
    contract(array);
}

