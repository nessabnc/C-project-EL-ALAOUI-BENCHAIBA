#include "adjlist.h"


static cell_t* create_cell(int to, float prob) {
    cell_t *c = (cell_t*)malloc(sizeof(cell_t));
    if (!c) {
        perror("malloc create_cell");
        exit(EXIT_FAILURE);
    }
    c->to = to;
    c->prob = prob;
    c->next = NULL;
    return c;
}


static adjacency_list_t create_empty_adjlist(int n) {
    adjacency_list_t g;
    g.n = n;
    g.lists = (list_t*)malloc(sizeof(list_t) * n);
    if (!g.lists) {
        perror("malloc lists");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; ++i) g.lists[i].head = NULL;
    return g;
}


void addEdgeAdj(adjacency_list_t *g, int from, int to, float prob) {
    if (!g) return;
    if (from < 1 || from > g->n || to < 1 || to > g->n) {
        fprintf(stderr, "addEdgeAdj: indices invalid (%d -> %d)\n", from, to);
        return;
    }
    cell_t *c = create_cell(to, prob);
    int idx = from - 1;
    c->next = g->lists[idx].head;
    g->lists[idx].head = c;
}

adjacency_list_t readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    adjacency_list_t g = create_empty_adjlist(nbvert);

    int start, end;
    float proba;
    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3) {
        addEdgeAdj(&g, start, end, proba);
    }
    fclose(file);
    return g;
}


void printAdjacencyList(const adjacency_list_t *g) {
    if (!g) return;
    printf("Adjacency list (n=%d):\n", g->n);
    for (int i = 0; i < g->n; ++i) {
        printf("List for vertex %d: [head]", i+1);
        cell_t *cur = g->lists[i].head;
        while (cur) {
            printf(" -> (%d, %.4f)", cur->to, cur->prob);
            cur = cur->next;
        }
        printf("\n");
    }
}


float outgoingSum(const adjacency_list_t *g, int vertex) {
    if (!g) return 0.0f;
    if (vertex < 1 || vertex > g->n) return 0.0f;
    float s = 0.0f;
    cell_t *cur = g->lists[vertex-1].head;
    while (cur) {
        s += cur->prob;
        cur = cur->next;
    }
    return s;
}


void freeAdjacencyList(adjacency_list_t *g) {
    if (!g) return;
    if (g->lists) {
        for (int i = 0; i < g->n; ++i) {
            cell_t *cur = g->lists[i].head;
            while (cur) {
                cell_t *next = cur->next;
                free(cur);
                cur = next;
            }
        }
        free(g->lists);
        g->lists = NULL;
    }
    g->n = 0;
}
