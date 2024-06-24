#ifndef ARRAYLIST
#define ARRAYLIST

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_CAPACITY 200

typedef struct Node {
  void *item;
} Node;

typedef struct ArrayList {
  Node *array;
  int size;
  int count;
} ArrayList;

//initialize empty
void initializeArrayList(ArrayList *list);
// push to the end of array
void pushItemArrayList(ArrayList *list, void *item, unsigned long int item_size);
//insert at position of the array
void insertAtArrayList(ArrayList *list, int idx, void *item, unsigned long int item_size);
//return item referer to the index 
void *atArrayList(ArrayList *list, int idx);
//remove
void removeItemInArray(ArrayList *list, int index_to_remove);

#endif
