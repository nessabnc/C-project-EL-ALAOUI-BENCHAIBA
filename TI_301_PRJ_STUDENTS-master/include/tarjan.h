#ifndef TARJAN_H
#define TARJAN_H

#include "adjlist.h"


typedef struct t_tarjan_vertex {
    int id;
    int index;
    int lowlink;
    int onstack;
} t_tarjan_vertex;

typedef struct t_class {
    int *vertices;
    int count;
    int capacity;
    char name[16];
} t_class;


typedef struct t_partition {
    t_class *classes;
    int count;
    int capacity;
} t_partition;


t_partition tarjan_partition(const adjacency_list_t *g);
void free_partition(t_partition *p);
int vertex_to_component_map(const t_partition *p, int n, int *map_out);

#endif
