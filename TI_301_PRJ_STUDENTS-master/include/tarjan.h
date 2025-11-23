#ifndef TARJAN_H
#define TARJAN_H

#include "adjlist.h"

/* Structure pour Tarjan : informations associées à chaque sommet */
typedef struct t_tarjan_vertex {
    int id;         /* identificateur du sommet (1..n) */
    int index;      /* numéro provisoire (initialisé à -1) */
    int lowlink;    /* accessible number */
    int onstack;    /* 0/1, si le sommet est sur la pile */
} t_tarjan_vertex;

/* structure représentant une composante (classe) */
typedef struct t_class {
    int *vertices;  /* tableau dynamique des sommets */
    int count;      /* nombre de sommets */
    int capacity;   /* capacité allouée */
    char name[16];  /* ex: "C1", "C2" */
} t_class;

/* partition = tableau de classes */
typedef struct t_partition {
    t_class *classes;
    int count;
    int capacity;
} t_partition;

/* prototypes */
t_partition tarjan_partition(const adjacency_list_t *g);
void free_partition(t_partition *p);
int vertex_to_component_map(const t_partition *p, int n, int *map_out); /* map_out size n (1..n) */

#endif /* TARJAN_H */
