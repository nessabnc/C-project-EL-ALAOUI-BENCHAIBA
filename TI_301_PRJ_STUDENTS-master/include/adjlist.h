#ifndef ADJLIST_H
#define ADJLIST_H

#include "common.h"

/* cellule représentant une arête (edge) dans la liste d'adjacence */
typedef struct cell {
    int to;                 /* sommet d'arrivée (1..n) */
    float prob;             /* probabilité (0 < prob <= 1) */
    struct cell *next;      /* pointeur vers la cellule suivante */
} cell_t;

/* liste pour chaque sommet (liste chaînée d'arêtes sortantes) */
typedef struct list {
    cell_t *head;
} list_t;

/* structure principale : tableau dynamique de listes + taille */
typedef struct adjacency_list {
    int n;          /* nombre de sommets */
    list_t *lists;  /* tableau de listes (index 1..n mapped to 0..n-1) */
} adjacency_list_t;

/* Prototypes */
adjacency_list_t readGraph(const char *filename);
void freeAdjacencyList(adjacency_list_t *g);
void printAdjacencyList(const adjacency_list_t *g);
void addEdgeAdj(adjacency_list_t *g, int from, int to, float prob);
float outgoingSum(const adjacency_list_t *g, int vertex);

#endif /* ADJLIST_H */
