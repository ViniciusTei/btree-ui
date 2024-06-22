#include "arraylist.h"

void initializeArrayList(ArrayList *list) {
  list->first = 0;
  list->last = list->first;
}

void mallocItemMemoryAndAddToList(ArrayList *list, void *item,
                                  unsigned long int item_size) {
  list->array[list->last].item = malloc(item_size);
  memcpy(list->array[list->last].item, item, item_size);
  list->last++;

  return;
}

void pushItemArrayList(ArrayList *list, void *item,
                       unsigned long int item_size) {
  if (list->last == list->first) {
    list->array = malloc(sizeof(Node));
    mallocItemMemoryAndAddToList(list, item, item_size);
    return;
  }

  // printf("\nlast %d", list->last);
  list->array = realloc(list->array, (list->last + 1) * sizeof(Node));
  mallocItemMemoryAndAddToList(list, item, item_size);

  return;
}

void insertAtArrayList(ArrayList *list, int idx, void *item,
                       unsigned long int item_size) {
  if (idx < 0) {
    printf("Error cannot insert at idx, out of bounds\n");
    return;
  }
  
  // If idx is beyond the last item
  if (idx >= list->last + 1) {
    list->array = realloc(list->array, (idx + 1) * sizeof(Node));
    if (list->array == NULL) {
      printf("Error: memory allocation failed\n");
      return;
    }
    for (int i = list->last + 1; i <= idx; i++) {
      list->array[i].item = malloc(item_size);
      if (list->array[i].item == NULL) {
        printf("Error: memory allocation failed\n");
        return;
      }
      list->array[i].item = NULL; // Initialize to NULL
    }
    list->last = idx; // Update the last index
  } else {
    // Allocate memory for the new item
    list->array = realloc(list->array, (list->last + 2) * sizeof(Node));
    if (list->array == NULL) {
      printf("Error: memory allocation failed\n");
      return;
    }

    // Shift elements to the right to make room for the new item
    for (int i = list->last + 1; i > idx; i--) {
      list->array[i] = list->array[i - 1];
    }

    // Allocate memory for the new item
    list->array[idx].item = malloc(item_size);
    if (list->array[idx].item == NULL) {
      printf("Error: memory allocation failed\n");
      return;
    }
  }

  // Insert the new item at idx
  memcpy(list->array[idx].item, item, item_size);

  // Update the last index
  list->last++;
  return;
}

void *atArrayList(ArrayList *list, int idx) {
  if (idx > list->last || idx < 0) {
    return NULL;
  }

  if (list->array == NULL) {
    return NULL;
  }

  return list->array[idx].item;
}

void removeItemInArray(ArrayList *list, int index_to_remove) {
  // lista esta vazia
  if (list->first == list->last || index_to_remove >= list->last) {
    printf("\nErro ao remover da lista. Lista vazia\n");
    return;
  }

  Node to_remove = list->array[index_to_remove];

  for (int aux = index_to_remove; aux < list->last - 1; aux++) {
    list->array[aux] = list->array[aux + 1];
  }
  list->last -= 1;
  free(to_remove.item);
};
