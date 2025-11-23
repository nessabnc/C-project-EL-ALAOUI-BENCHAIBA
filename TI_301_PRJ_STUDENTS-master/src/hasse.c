#include "hasse.h"

/* helpers pour t_link_array */
static void link_array_init(t_link_array *la) {
    la->capacity = 8;
    la->count = 0;
    la->links = MALLOC_OR_DIE(t_link, la->capacity);
}
static void link_array_add(t_link_array *la, int s, int e) {
    /* empêche le doublon */
    for (int i = 0; i < la->count; ++i) {
        if (la->links[i].start == s && la->links[i].end == e) return;
    }
    if (la->count >= la->capacity) {
        la->capacity *= 2;
        la->links = (t_link*)realloc(la->links, sizeof(t_link) * la->capacity);
        if (!la->links) { perror("realloc links"); exit(EXIT_FAILURE); }
    }
    la->links[la->count].start = s;
    la->links[la->count].end = e;
    la->count++;
}

/* construction des liens inter-classes (algorithme donné dans le sujet) */
t_link_array build_hasse_links(const adjacency_list_t *g, const t_partition *part) {
    int n = g->n;
    t_link_array la;
    link_array_init(&la);

    /* table sommet -> classe */
    int *map = MALLOC_OR_DIE(int, n);
    vertex_to_component_map(part, n, map);

    for (int i = 1; i <= n; ++i) {
        int Ci = map[i-1];
        cell_t *cur = g->lists[i-1].head;
        while (cur) {
            int j = cur->to;
            int Cj = map[j-1];
            if (Ci != Cj) {
                link_array_add(&la, Ci, Cj);
            }
            cur = cur->next;
        }
    }

    SAFE_FREE(map);
    return la;
}

void free_link_array(t_link_array *la) {
    if (!la) return;
    SAFE_FREE(la->links);
    la->count = 0;
    la->capacity = 0;
}

/* Écriture du diagramme Hasse au format Mermaid (flowchart LR) */
void write_hasse_mermaid(const t_partition *part, const t_link_array *links, const char *outfilename) {
    FILE *f = fopen(outfilename, "wt");
    if (!f) {
        perror("open mermaid out");
        return;
    }
    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\n");
    fprintf(f, "flowchart LR\n");

    /* écriture des noeuds C1, C2... avec leurs sommets */
    for (int i = 0; i < part->count; ++i) {
        /* affichage ex: C1{\"{1,5,7}\"} ou C1((C1)) selon préférence */
        fprintf(f, "C%d((%s))\n", i+1, part->classes[i].name);
        /* On peut aussi afficher l'ensemble des sommets (optionnel) */
        fprintf(f, "subgraph C%d_sub[\"%s: {", i+1, part->classes[i].name);
        for (int j = 0; j < part->classes[i].count; ++j) {
            fprintf(f, "%d", part->classes[i].vertices[j]);
            if (j+1 < part->classes[i].count) fprintf(f, ",");
        }
        fprintf(f, "}\"]\nend\n");
    }

    /* liens */
    for (int i = 0; i < links->count; ++i) {
        int s = links->links[i].start;
        int e = links->links[i].end;
        fprintf(f, "C%d --> C%d\n", s+1, e+1);
    }
    fclose(f);
}

/* Optionnel : suppression d'arêtes transitives (algorithme naïf O(m^2+n*m)).
   Prototype fourni dans le sujet. */
void remove_transitive_links(t_link_array *la, int num_classes) {
    /* On construit une matrice d'adjacence binaire entre classes */
    int k = num_classes;
    int *mat = MALLOC_OR_DIE(int, k * k);
    for (int i = 0; i < k * k; ++i) mat[i] = 0;
    for (int i = 0; i < la->count; ++i) {
        int s = la->links[i].start;
        int e = la->links[i].end;
        mat[s*k + e] = 1;
    }
    /* Floyd-Warshall pour fermeture transitive */
    for (int m = 0; m < k; ++m)
        for (int i = 0; i < k; ++i)
            if (mat[i*k + m])
                for (int j = 0; j < k; ++j)
                    if (mat[m*k + j])
                        mat[i*k + j] = 1;

    /* on supprime liaisons (i,j) s'il existe un m différent tel que i->m->j */
    t_link_array newla;
    link_array_init(&newla);
    for (int i = 0; i < la->count; ++i) {
        int s = la->links[i].start;
        int e = la->links[i].end;
        int redundant = 0;
        for (int m = 0; m < k; ++m) {
            if (m == s || m == e) continue;
            if (mat[s*k + m] && mat[m*k + e]) { redundant = 1; break; }
        }
        if (!redundant) link_array_add(&newla, s, e);
    }

    /* swap */
    SAFE_FREE(la->links);
    *la = newla;
    SAFE_FREE(mat);
}
