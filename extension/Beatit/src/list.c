#include "list.h"
#include <stdlib.h>
#include <stdio.h>

struct list_elem
{
  void *value;
  struct list_elem *prev;
  struct list_elem *next;
};

static list_elem* alloc_list_elem(void) {
  list_elem* elem = malloc(sizeof(list_elem));

  if (elem == NULL) {
    perror("alloc_list_elem");
    exit(EXIT_FAILURE);
  }

  return elem;
}

static void list_free_elem(list_elem* elem) {
  free(elem);
}

list_elem* list_get_first(list* lst) {
  return lst->head->next == lst->tail ? NULL : lst->head->next;
}

list_elem* list_get_next(list_elem* elem) {
  return elem->next->value == NULL ? NULL : elem->next;
}

list* create_list(void) {
  list* lst = malloc(sizeof(list));

  lst->head = alloc_list_elem();
  lst->tail = alloc_list_elem();

  lst->head->value = NULL;
  lst->head->prev = NULL;
  lst->head->next = lst->tail;

  lst->tail->value = NULL;
  lst->tail->prev = lst->head;
  lst->tail->next = NULL;

  return lst;
}

void list_insert(list* lst, void* value) {
  list_elem* new_elem = alloc_list_elem();
  new_elem->value = value;

  list_elem* before_elem = lst->tail->prev;
  before_elem->next = new_elem;
  new_elem->next = lst->tail;
  new_elem->prev = before_elem;
  lst->tail->prev = new_elem;
  lst->size++;
}

int list_remove(list* lst, void* value) {
  list_elem* curr_elem = list_get_first(lst);

  while (curr_elem != NULL) {
    if ((curr_elem->value) == value) {
      curr_elem->prev->next = curr_elem->next;
      curr_elem->next->prev = curr_elem->prev;
      list_free_elem(curr_elem);
      (lst->size)--;
      return 1;
    }

    curr_elem = list_get_next(curr_elem);
  }

  return 0;
}

void list_destroy(list* lst) {
  list_elem* curr_elem = list_get_first(lst);
  list_elem* next_elem;

  while (curr_elem != NULL) {
    next_elem = curr_elem->next;
    list_free_elem(curr_elem);
    curr_elem = next_elem;
  }
}

void* list_pop(list* lst) {
  list_elem* first = list_get_first(lst);

  if (first == NULL) {
    return NULL;
  }

  lst->head->next = first->next;
  first->next->prev = lst->head;

  void* value = first->value;
  list_free_elem(first);
  return value;
}

void list_test(void) {
  char* str1 = "String1";
  char* str2 = "String2";
  char* str3 = "String3";

  list* lst = create_list();
  list_insert(lst, str1);
  list_insert(lst, str2);
  list_insert(lst, str3);

  printf("First element: %s\n", (char*) lst->head->next->value);
  printf("Second element: %s\n", (char*) lst->head->next->next->value);

  list_remove(lst, str1);
  list_remove(lst, str2);

  printf("First element: %s\n", (char*) lst->head->next->value);
}
