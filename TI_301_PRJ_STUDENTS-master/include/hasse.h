#ifndef HASSE_H
#define HASSE_H

#include "tarjan.h"


typedef struct t_link {
    int start;
    int end;
} t_link;


typedef struct t_link_array {
    t_link *links;
    int count;
    int capacity;
} t_link_array;


t_link_array build_hasse_links(const adjacency_list_t *g, const t_partition *part);
void free_link_array(t_link_array *la);
void write_hasse_mermaid(const t_partition *part, const t_link_array *links, const char *outfilename);
void remove_transitive_links(t_link_array *la, int num_classes); /* optionnel */
#endif