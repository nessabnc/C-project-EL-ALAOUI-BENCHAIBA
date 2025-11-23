#define _GNU_SOURCE
#include "tarjan.h"


typedef struct int_stack {
    int *data;
    int top;
    int capacity;
} int_stack;

static void stack_init(int_stack *s, int cap) {
    s->data = MALLOC_OR_DIE(int, cap);
    s->top = 0;
    s->capacity = cap;
}
static void stack_push(int_stack *s, int v) {
    if (s->top >= s->capacity) {
        s->capacity *= 2;
        s->data = (int*)realloc(s->data, sizeof(int) * s->capacity);
        if (!s->data) { perror("realloc stack"); exit(EXIT_FAILURE); }
    }
    s->data[s->top++] = v;
}
static int stack_pop(int_stack *s) {
    if (s->top == 0) return -1;
    return s->data[--s->top];
}
static int stack_peek(int_stack *s) {
    if (s->top == 0) return -1;
    return s->data[s->top - 1];
}
static int stack_empty(int_stack *s) { return s->top == 0; }
static void stack_free(int_stack *s) { SAFE_FREE(s->data); }


static void class_init(t_class *c, int capacity, const char *name) {
    c->vertices = MALLOC_OR_DIE(int, capacity);
    c->count = 0;
    c->capacity = capacity;
    strncpy(c->name, name, 15); c->name[15] = '\0';
}
static void class_add_vertex(t_class *c, int v) {
    if (c->count >= c->capacity) {
        c->capacity *= 2;
        c->vertices = (int*)realloc(c->vertices, sizeof(int) * c->capacity);
        if (!c->vertices) { perror("realloc class vertices"); exit(EXIT_FAILURE); }
    }
    c->vertices[c->count++] = v;
}
static void class_free(t_class *c) { SAFE_FREE(c->vertices); }


static void partition_init(t_partition *p) {
    p->capacity = 8;
    p->count = 0;
    p->classes = MALLOC_OR_DIE(t_class, p->capacity);
}
static void partition_add_class(t_partition *p, t_class *c) {
    if (p->count >= p->capacity) {
        p->capacity *= 2;
        p->classes = (t_class*)realloc(p->classes, sizeof(t_class) * p->capacity);
        if (!p->classes) { perror("realloc partition"); exit(EXIT_FAILURE); }
    }
    p->classes[p->count++] = *c;
}


static void strongconnect(int v, adjacency_list_t const *g,
                          t_tarjan_vertex *V, int *index_counter,
                          int_stack *S, t_partition *result) {
    V[v-1].index = *index_counter;
    V[v-1].lowlink = *index_counter;
    (*index_counter)++;
    stack_push(S, v);
    V[v-1].onstack = 1;


    cell_t *cur = g->lists[v-1].head;
    while (cur) {
        int w = cur->to;
        if (V[w-1].index == -1) {
            strongconnect(w, g, V, index_counter, S, result);
            if (V[w-1].lowlink < V[v-1].lowlink)
                V[v-1].lowlink = V[w-1].lowlink;
        } else if (V[w-1].onstack) {
            if (V[w-1].index < V[v-1].lowlink)
                V[v-1].lowlink = V[w-1].index;
        }
        cur = cur->next;
    }


    if (V[v-1].lowlink == V[v-1].index) {
        t_class c;
        class_init(&c, 4, "C?");
        while (1) {
            int w = stack_pop(S);
            if (w == -1) break;
            V[w-1].onstack = 0;
            class_add_vertex(&c, w);
            if (w == v) break;
        }

        int k = result->count + 1;
        snprintf(c.name, sizeof(c.name), "C%d", k);
        partition_add_class(result, &c);
    }
}


t_partition tarjan_partition(const adjacency_list_t *g) {
    t_partition result;
    partition_init(&result);

    int n = g->n;
    t_tarjan_vertex *V = MALLOC_OR_DIE(t_tarjan_vertex, n);
    for (int i = 0; i < n; ++i) {
        V[i].id = i+1;
        V[i].index = -1;
        V[i].lowlink = -1;
        V[i].onstack = 0;
    }

    int_stack S;
    stack_init(&S, n > 0 ? n : 4);

    int index_counter = 0;
    for (int v = 1; v <= n; ++v) {
        if (V[v-1].index == -1) {
            strongconnect(v, g, V, &index_counter, &S, &result);
        }
    }


    stack_free(&S);
    free(V);
    return result;
}


void free_partition(t_partition *p) {
    if (!p) return;
    for (int i = 0; i < p->count; ++i) {
        class_free(&p->classes[i]);
    }
    SAFE_FREE(p->classes);
    p->count = 0;
    p->capacity = 0;
}


int vertex_to_component_map(const t_partition *p, int n, int *map_out) {
    if (!p || !map_out) return -1;
    for (int i = 0; i < n; ++i) map_out[i] = -1;
    for (int ci = 0; ci < p->count; ++ci) {
        for (int j = 0; j < p->classes[ci].count; ++j) {
            int v = p->classes[ci].vertices[j];
            map_out[v-1] = ci;
        }
    }
    return 0;
}
