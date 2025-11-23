#include "matrix.h"


t_matrix createEmptyMatrix(int n) {
    t_matrix m;
    m.rows = n;
    m.cols = n;
    m.data = MALLOC_OR_DIE(float*, n);
    for (int i = 0; i < n; ++i) {
        m.data[i] = MALLOC_OR_DIE(float, n);
        for (int j = 0; j < n; ++j) m.data[i][j] = 0.0f;
    }
    return m;
}

void freeMatrix(t_matrix *m) {
    if (!m) return;
    for (int i = 0; i < m->rows; ++i) SAFE_FREE(m->data[i]);
    SAFE_FREE(m->data);
    m->rows = m->cols = 0;
}


t_matrix adjacency_to_matrix(const adjacency_list_t *g) {
    int n = g->n;
    t_matrix M = createEmptyMatrix(n);
    for (int i = 1; i <= n; ++i) {
        cell_t *cur = g->lists[i-1].head;
        while (cur) {
            int j = cur->to;
            M.data[i-1][j-1] += cur->prob;
            cur = cur->next;
        }
    }
    return M;
}

t_matrix copyMatrix(const t_matrix *src) {
    t_matrix dest = createEmptyMatrix(src->rows);
    for (int i = 0; i < src->rows; ++i)
        for (int j = 0; j < src->cols; ++j)
            dest.data[i][j] = src->data[i][j];
    return dest;
}


void multiplyMatrices(const t_matrix *A, const t_matrix *B, t_matrix *out) {
    int n = A->rows;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            out->data[i][j] = 0.0f;

    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            float aik = A->data[i][k];
            if (aik == 0.0f) continue;
            for (int j = 0; j < n; ++j) {
                out->data[i][j] += aik * B->data[k][j];
            }
        }
    }
}

float diffMatrix(const t_matrix *A, const t_matrix *B) {
    int n = A->rows;
    float s = 0.0f;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            s += fabsf(A->data[i][j] - B->data[i][j]);
    return s;
}


t_matrix powerMatrix(const t_matrix *M, int p) {
    int n = M->rows;
    t_matrix result = createEmptyMatrix(n);
    t_matrix temp = createEmptyMatrix(n);

    if (p == 0) {
        for (int i = 0; i < n; ++i) result.data[i][i] = 1.0f;
        freeMatrix(&temp);
        return result;
    }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            result.data[i][j] = M->data[i][j];

    for (int step = 1; step < p; ++step) {
        multiplyMatrices(&result, M, &temp);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                result.data[i][j] = temp.data[i][j];
    }
    freeMatrix(&temp);
    return result;
}


t_matrix subMatrix(const t_matrix *matrix, const t_partition *part, int compo_index) {
    t_class *c = &part->classes[compo_index];
    int k = c->count;
    t_matrix sm = createEmptyMatrix(k);
    for (int i = 0; i < k; ++i) {
        int vi = c->vertices[i] - 1; /* indice dans matrice globale */
        for (int j = 0; j < k; ++j) {
            int vj = c->vertices[j] - 1;
            sm.data[i][j] = matrix->data[vi][vj];
        }
    }
    return sm;
}

void printMatrix(const t_matrix *m) {
    printf("Matrix %dx%d:\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; ++i) {
        for (int j = 0; j < m->cols; ++j) {
            printf("%8.4f ", m->data[i][j]);
        }
        printf("\n");
    }
}
