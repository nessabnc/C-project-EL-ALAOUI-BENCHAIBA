#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define EPS_MARKOV_LOWER 0.99f
#define EPS_MARKOV_UPPER 1.01f


#define MALLOC_OR_DIE(t, n) ((t*)malloc(sizeof(t) * (n)))
#define SAFE_FREE(p) do { if (p) { free(p); p = NULL; } } while(0)

#endif
