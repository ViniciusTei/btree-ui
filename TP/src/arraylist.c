#include "arraylist.h"
#include <string.h>

void initializeArrayList(ArrayList *list) {
  list->size = 0;
  list->count = 0;
  list->array = NULL;
}

void mallocItemMemoryAndAddToList(ArrayList *list, void *item,
                                  unsigned long int item_size) {
  list->array[list->count].item = malloc(item_size);
  memcpy(list->array[list->count].item, item, item_size);
  list->count++;

  return;
}

void pushItemArrayList(ArrayList *list, void *item,
                       unsigned long int item_size) {
  if (list->count == list->size) {
    list->size = list->size + 20;
    list->array = realloc(list->array, (list->size) * sizeof(Node));

    if (list->array == NULL) {
      printf("Error: memmory allocation failed\n");
      return;
    }
  }

  mallocItemMemoryAndAddToList(list, item, item_size);

  return;
}

void insertAtArrayList(ArrayList *list, int idx, void *item,
                       unsigned long int item_size) {
  if (idx < 0) {
    printf("Error: cannot insert at idx, out of bounds\n");
    return;
  }

  // If the idx is beyond the current size, we need to expand the array
  if (idx >= list->size) {
    list->array = realloc(list->array, (idx + 1) * sizeof(Node));
    if (list->array == NULL) {
      printf("Error: memory allocation failed\n");
      return;
    }
    
    for (int i = list->size; i <= idx; i++) {
      list->array[i].item = malloc(item_size);
      if (list->array[i].item == NULL) {
        printf("Error: memory allocation failed\n");
        return;
      }
      list->array[i].item = NULL; // Initialize to NULL
    }
    list->size = idx + 1; // Update the total allocated size
  }

  // If the idx is within the array but no element has been inserted there
  if (list->array[idx].item == NULL) {
    list->array[idx].item = malloc(item_size);
    if (list->array[idx].item == NULL) {
      printf("Error: memory allocation failed\n");
      return;
    }
  }

  memcpy(list->array[idx].item, item, item_size);
  list->count++;
}

void *atArrayList(ArrayList *list, int idx) {
  if (list == NULL) {
    printf("Error: list is NULL\n");
    return NULL;
  }

  if (idx < 0 || idx >= list->count) {
    printf("Error: index out of bounds\n");
    return NULL;
  }

  if (list->array == NULL) {
    printf("Error: array is NULL\n");
    return NULL;
  }

  if (list->array[idx].item == NULL) {
    printf("Error: item is NULL\n");
    return NULL;
  }

  return list->array[idx].item;
}

void removeItemInArray(ArrayList *list, int index_to_remove) {
  // lista esta vazia
  if (list->count == 0 || index_to_remove >= list->count) {
    printf("\nErro ao remover da lista. Lista vazia\n");
    return;
  }

  Node to_remove = list->array[index_to_remove];

  for (int aux = index_to_remove; aux < list->count - 1; aux++) {
    list->array[aux] = list->array[aux + 1];
  }
  list->count -= 1;
  free(to_remove.item);
};
