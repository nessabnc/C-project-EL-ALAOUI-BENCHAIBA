#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int maint(){
  typedef struct s_cell{
    int value;
    struct s_cell *next;
  }t_cell;

  typedef struct s_list{
    t_cell *first;
    struct s_list *next;
  }t_list;

 t_cell *createCell(int value){
   t_cell *newCell = (t_cell *)malloc(sizeof(t_cell));
   newCell->value = value;
   newCell->next = NULL;
   return newCell;
 }

 t_list *createList(){
   t_list *newList = (t_list *)malloc(sizeof(t_list));
   newList->head = NULL;
   newList->tail = NULL;
   return newList;
 }

void add_cell(t_list *list, t_cell *cell){
  t_cell *newCell = createCell(cell->value);
  if(list->head == NULL){
    list->head = newCell;
    list->tail = newCell;
    return;
  }
}

 void displayList(t_list *head){
   t_list *current = head;
   while(current != NULL){
     printf("%d ", current->value);
     current = current->next
   }
 }

