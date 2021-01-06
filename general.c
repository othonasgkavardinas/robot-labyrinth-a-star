#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <stdarg.h>
#include <math.h>


#define print_matrix(matrix, type) ({ \
    int i, j; \
    for (i = 0; i < N; i++, puts("")) \
        for (j = 0; j < N; j++) \
            printf(type==IS_INT?"%d ":"%.2f ", matrix[i][j]); \
})

#define alloc_struct(type, var, no) ({ \
		*var = (type *) malloc (no*sizeof(type)); \
		if(*var == NULL) { \
			printf("\nError! malloc.\n"); \
			exit(1); \
		} \
        \
})

typedef int vector[2];

enum { FALSE, TRUE };

void copy_vector_from(vector *v1, vector *v2) {
    memcpy(*v1, *v2, sizeof(vector));
}

int equals_vector(vector v1, vector v2) {
    return v1[0]==v2[0] && v1[1]==v2[1];
}

int print_vector(vector v) {
    printf("(%d, %d)\n", v[0], v[1]);
}

void set_int_array(int *array, int size, ...) {
    int i;
    va_list args;
    va_start(args, size);
    for(i=0; i<size; i++)
        array[i] = va_arg(args, int);
    va_end(args);
}

void set_vector_array_from_pairs(vector *array, int size, ...) {
    int i, j;
    va_list args;
    va_start(args, size);
    for(i=0; i<size; i++)
      for(j=0; j<2; j++)
        array[i][j] = va_arg(args, int);
    va_end(args);
}
