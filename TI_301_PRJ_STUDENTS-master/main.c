#include "include/adjlist.h"
#include "include/tarjan.h"
#include "include/hasse.h"
#include "include/matrix.h"
#include "include/common.h"

#include <time.h>

/* helper: vérification Markov : chaque ligne (sommet) somme entre 0.99 et 1.01 */
static int check_markov(const adjacency_list_t *g) {
    int ok = 1;
    for (int i = 1; i <= g->n; ++i) {
        float s = outgoingSum(g, i);
        if (s < EPS_MARKOV_LOWER || s > EPS_MARKOV_UPPER) {
            printf("The graph is not a Markov graph: sum of probabilities of vertex %d is %.4f\n", i, s);
            ok = 0;
        }
    }
    if (ok) printf("The graph is a Markov graph.\n");
    return ok;
}

/* export adjacency -> mermaid format (pour partie 1) */
static void write_mermaid_from_adj(const adjacency_list_t *g, const char *outfilename) {
    FILE *f = fopen(outfilename, "wt");
    if (!f) { perror("open mermaid out"); return; }
    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\n");
    fprintf(f, "flowchart LR\n");
    /* nodes A((1)) B((2)) ... ; helper getId : A B C ... AA AB ... */
    /* simple: use labels N1((1)) etc. */
    for (int i = 1; i <= g->n; ++i) {
        fprintf(f, "N%d((%d))\n", i, i);
    }
    /* edges */
    for (int i = 1; i <= g->n; ++i) {
        cell_t *cur = g->lists[i-1].head;
        while (cur) {
            fprintf(f, "N%d -->|%.2f| N%d\n", i, cur->prob, cur->to);
            cur = cur->next;
        }
    }
    fclose(f);
    printf("Mermaid adjacency exported to %s\n", outfilename);
}

/* calcule distributions stationnaires par composante en élevant les sous-matrices à puissance p
   jusqu'à convergence (diff < eps) ou pmax atteint */
static void compute_stationary_per_component(const t_partition *part, const t_matrix *M, float eps, int pmax) {
    printf("\n--- Stationary distributions per component ---\n");
    for (int ci = 0; ci < part->count; ++ci) {
        t_class *c = &part->classes[ci];
        if (c->count == 0) continue;
        t_matrix sm = subMatrix(M, part, ci);
        /* calculer sm^p pour p=1.. jusqu'à convergence des lignes (ou différence globale) */
        t_matrix prev = copyMatrix(&sm);
        t_matrix cur = copyMatrix(&sm);
        int p = 1;
        while (p <= pmax) {
            t_matrix next = createEmptyMatrix(sm.rows);
            multiplyMatrices(&cur, &sm, &next);
            float d = diffMatrix(&next, &cur);
            /* si les lignes de next sont (presque) toutes identiques -> stationnaire */
            int rows_equal = 1;
            for (int i = 1; i < next.rows; ++i) {
                for (int j = 0; j < next.cols; ++j) {
                    if (fabsf(next.data[i][j] - next.data[0][j]) > eps) { rows_equal = 0; break; }
                }
                if (!rows_equal) break;
            }
            copyMatrix(&next); /* juste pour respecter l'usage (on gère copie manuellement) */
            freeMatrix(&cur);
            cur = next;
            if (rows_equal || d < eps) break;
            p++;
        }
        /* résultat approximatif : ligne 0 de cur (ou moyenne des lignes) */
        printf("Component %s (size %d):\n", c->name, c->count);
        /* moyenne des lignes pour robustesse */
        for (int j = 0; j < cur.cols; ++j) {
            float s = 0.0f;
            for (int i = 0; i < cur.rows; ++i) s += cur.data[i][j];
            float val = s / cur.rows;
            printf("%.4f ", val);
        }
        printf("\n");
        freeMatrix(&prev);
        freeMatrix(&cur);
        freeMatrix(&sm);
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s graph_input.txt out/graph.mmd\n", argv[0]);
        return 1;
    }
    const char *input = argv[1];
    const char *mermaid_out = argv[2];

    /* 1) lecture du graphe */
    adjacency_list_t g = readGraph(input);
    printAdjacencyList(&g);

    /* 2) vérification Markov */
    check_markov(&g);

    /* 3) export mermaid du graphe */
    write_mermaid_from_adj(&g, mermaid_out);

    /* 4) Tarjan : partition en classes */
    t_partition part = tarjan_partition(&g);
    printf("\nPartition: %d components found.\n", part.count);
    for (int i = 0; i < part.count; ++i) {
        printf("Component %s: {", part.classes[i].name);
        for (int j = 0; j < part.classes[i].count; ++j) {
            printf("%d", part.classes[i].vertices[j]);
            if (j+1 < part.classes[i].count) printf(",");
        }
        printf("}\n");
    }

    /* 5) Hasse links and mermaid */
    t_link_array links = build_hasse_links(&g, &part);
    /* optional: remove_transitive_links(&links, part.count); */
    write_hasse_mermaid(&part, &links, "out/hasse.mmd");
    printf("Hasse exported to out/hasse.mmd (with %d links)\n", links.count);

    /* 6) caractéristiques (transitoire / persistante / absorbing / irreducible) */
    printf("\n--- Graph characteristics ---\n");
    int irreducible = (part.count == 1);
    if (irreducible) printf("Graph is irreducible (single class).\n");
    else printf("Graph is NOT irreducible (multiple classes).\n");

    /* classes persistent if no outgoing arrow from cette classe (no link start==ci) */
    for (int ci = 0; ci < part.count; ++ci) {
        int has_out = 0;
        for (int i = 0; i < links.count; ++i) {
            if (links.links[i].start == ci) { has_out = 1; break; }
        }
        if (has_out) {
            printf("%s is transient (has outgoing links)\n", part.classes[ci].name);
        } else {
            printf("%s is persistent (no outgoing links)\n", part.classes[ci].name);
            if (part.classes[ci].count == 1) {
                printf(" -> it's absorbing (single state in persistent class)\n");
            }
        }
    }

    /* 7) matrices (Part 3) */
    t_matrix M = adjacency_to_matrix(&g);
    printMatrix(&M);
    /* exemples: calculer M^3 et M^7 (affichage) */
    t_matrix M3 = powerMatrix(&M, 3);
    printf("\nM^3:\n"); printMatrix(&M3);
    t_matrix M7 = powerMatrix(&M, 7);
    printf("\nM^7:\n"); printMatrix(&M7);
    freeMatrix(&M3); freeMatrix(&M7);

    /* 8) stationnaires par composante */
    compute_stationary_per_component(&part, &M, 0.01f, 200);

    /* cleanup */
    freeMatrix(&M);
    free_link_array(&links);
    free_partition(&part);
    freeAdjacencyList(&g);
    return 0;
}
