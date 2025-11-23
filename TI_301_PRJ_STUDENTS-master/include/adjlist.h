#ifndef ADJLIST_H
#define ADJLIST_H

#include "common.h"


typedef struct cell {
    int to;
    float prob;
    struct cell *next;
} cell_t;


typedef struct list {
    cell_t *head;
} list_t;


typedef struct adjacency_list {
    int n;
    list_t *lists;
} adjacency_list_t;


adjacency_list_t readGraph(const char *filename);
void freeAdjacencyList(adjacency_list_t *g);
void printAdjacencyList(const adjacency_list_t *g);
void addEdgeAdj(adjacency_list_t *g, int from, int to, float prob);
float outgoingSum(const adjacency_list_t *g, int vertex);

#endif
