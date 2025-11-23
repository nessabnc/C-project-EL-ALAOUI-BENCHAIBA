#ifndef MATRIX_H
#define MATRIX_H

#include "common.h"
#include "adjlist.h"
#include "tarjan.h"

typedef struct t_matrix {
    int rows;
    int cols;
    float **data;
} t_matrix;


t_matrix createEmptyMatrix(int n);
void freeMatrix(t_matrix *m);
t_matrix adjacency_to_matrix(const adjacency_list_t *g);
t_matrix copyMatrix(const t_matrix *src);
void multiplyMatrices(const t_matrix *A, const t_matrix *B, t_matrix *out);
float diffMatrix(const t_matrix *A, const t_matrix *B);
t_matrix powerMatrix(const t_matrix *M, int p);
t_matrix subMatrix(const t_matrix *matrix, const t_partition *part, int compo_index);
void printMatrix(const t_matrix *m);
#endif
