#ifndef LIST_H
#define LIST_H


typedef struct List_Elem {
  void* value;
  struct List_Elem* prev;
  struct List_Elem* next;
} list_elem;

typedef struct List {
  list_elem* head;
  list_elem* tail;
  int size;
} list;

list_elem* list_get_first(list* lst);

list_elem* list_get_next(list_elem* elem);

list* create_list();

void list_insert(list* lst, void* value);

int list_remove(list* lst, void* value);

void list_destroy(list* lst);

void* list_pop(list* lst);

void list_test();

#endif
