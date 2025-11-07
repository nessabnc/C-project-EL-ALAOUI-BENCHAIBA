#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <hasse.h>


 t_cell *createCell(int arrival, float probability) {
   t_cell *newCell = (t_cell *)malloc(sizeof(t_cell));
   newCell->arrival = arrival;
   newCell->probability = probability;
   newCell->next = NULL;
   return newCell;
 }

 t_list createemptyList(void){
   t_list *newList = (t_list *)malloc(sizeof(t_list));
   newList->head = NULL;
   return newList;
 }



void add_cell(t_list *list, int arrival, float probability){
  t_cell *newCell = createCell(arrival,probability);
  if(list->head == NULL){
    list->head = newCell;
    return;
  }
}

 void displayList(t_list list){
   t_list *current = list->head;
   while(current != NULL){
     printf("%d, %d ", current->arrival, current->probability);
     current = current->next;
   }
 }

   t_adjlist createEmptyAdjList(int size){
   t_adjlist adj;
   adj->size = size;
   adj->list = (t_list *)malloc(size * sizeof(t_list));
   for(int i=0; i<size; i++){
     adj.list[i] = createEmptyList();
   }
   return adj;
 }

  void displayAdjList(t_adjlist adj){
   for(int i=0; i<adj.size; i++){
     printf("List vertex %d: ", i+1);
     displayList(adj.list[i]);
   }
 }

  t_adjlist readGraph(const char *filename){
   FILE *file = fopen(filename, "rt");
   if(file == NULL){
     perror("Could not open file for reading");
     exit(EXIT_FAILURE);
   }
   int nbvert, start, end;
   int probability;
   if(fscanf(file, "%d", &nbvert) != 1){
     printf("Error reading number of vertices.\n");
     exit(1);
   }
   t_adjlist adj = createEmptyAdjList(nbvert);
   while(fscanf(file, "%d %d %f", &start, &end, &probability) == 3){
     addCell(&adj.list[start - 1], end, probability);
   }
   fclose(file);
   return adj;
 }

int checkMarkovGraph(t_adjlist adj){
   int ok = 1;
   for(int i=0; i<adj.size; i++){
     float sum = 0;
     t_cell *current = adj.list[i].head;
     while(current != NULL){
       sum += current->probability;
       current = current->next;
     }
     if(sum < 0.99 || sum > 1.01){
       printf("the sum of the probabilities of vertex %d is %.3f\n", i+1, sum);
       ok = 0;
     }
   }
   if(ok == 1){
     printf("The graph is a Markov graph\n");
   }else{
     printf("The graph is not a Markov graph\n");
   }
   return ok;
 }




