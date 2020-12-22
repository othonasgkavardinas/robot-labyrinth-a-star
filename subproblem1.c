#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <memory.h>
#include <stdarg.h>

#define N     5
#define P     0.7
#define MAX   100

#define G1    2
#define G2    3
#define T     4
#define PASSED    7

#define IS_INT   1
#define IS_FLOAT 0

#define print_matrix(matrix, type) ({ \
    int i, j; \
    for (i = 0; i < N; i++, puts("")) \
        for (j = 0; j < N; j++) \
            printf(type==IS_INT?"%d ":"%.2f ", matrix[i][j]); \
})

#define allocStruct(type, var, no) ({ \
		*var = (type *) malloc (no*sizeof(type)); \
		if(*var == NULL) { \
			printf("\nError! malloc.\n"); \
			exit(1); \
		} \
})

typedef int vector[2];

typedef struct {
    vector v;
    vector parent;
    float f;
} node;

float a_star(int [][N], float [][N], vector, vector, int *);
void alloc_nodes(node **, node **);
void init_start_node(node *, vector, float [][N]);
void copy_node_from(node *, node *);
void find_min_node(node *, int *, node [], int);
void remove_min_node(int, node *, int);
void realloc_node_array_if_needed(node *, int, int *);
int equals_vector(vector, vector);
void set_temp_parent_and_total_cost(vector *, node, float *);
void set_int_array(int *, int, ...);
void set_vector_array_from_pairs(vector *, int, ...);
void delete_node(node *, int, int, node);
void copy_vector_from(vector *, vector *);

void create_matrix_with_boundaries(int [][N]);
void print_options(int [][N]);
void scan_for(vector, char *);
void input_options(vector, vector, vector);
void set_options(int [][N], vector, vector, vector);
float do_manhattan(vector, vector);
void calculate_distances(float [][N], vector);

float a_star(int matrix[][N], float dists[][N], vector s, vector fast, int *extensions) {
    int open_size=0, open_size_max=N;
    int close_size=0, close_size_max=N;
    int min_position;
    node *open, *close;
    node min_node, start_node, direction_node, found, old;
    float total_cost = 0;
    vector temp_parent;
    int i;
    int conditions_size, conditions[conditions_size];
    vector direction_vectors[conditions_size];

    alloc_nodes(&open, &close);
    init_start_node(&(open[open_size++]), s, dists);

    while (1) {
        if (open_size == 0)
            break;
        find_min_node(&min_node, &min_position, open, open_size--);
        remove_min_node(min_position, open, open_size);
        realloc_node_array_if_needed(close, close_size, &close_size_max);
        close[close_size++] = min_node;
        if (equals_vector(min_node.v, fast)) {
            *extensions = close_size;
            matrix[min_node.parent[0]][min_node.parent[1]] = PASSED;
            set_temp_parent_and_total_cost(&temp_parent, min_node, &total_cost);
            while (1) {
                for (i=0; i<close_size; i++)
                    if (equals_vector(close[i].v, temp_parent)) {
                        copy_node_from(&found, &(close[i]));
                        set_temp_parent_and_total_cost(&temp_parent, close[i], &total_cost);
                        break;
                    }
                if (equals_vector(temp_parent, s))
                    break;
                matrix[found.parent[0]][found.parent[1]] = PASSED;
            }
            break;
        } else {
            conditions_size = 4;
            set_int_array(conditions, 4,
                min_node.v[0] != 0 && matrix[(min_node.v[0]) - 1][min_node.v[1]] != 1,
                min_node.v[0] != N && matrix[(min_node.v[0]) + 1][min_node.v[1]] != 1,
                min_node.v[1] != 0 && matrix[min_node.v[0]][(min_node.v[1]) - 1] != 1,
                min_node.v[1] != N && matrix[min_node.v[0]][(min_node.v[1]) + 1] != 1);
            set_vector_array_from_pairs(direction_vectors, 4,
                min_node.v[0]-1, min_node.v[1],
                min_node.v[0]+1, min_node.v[1],
                min_node.v[0], min_node.v[1]-1,
                min_node.v[0], min_node.v[1]+1);

            for(i=0; i<conditions_size; i++)
                if(conditions[i]) {
                    copy_vector_from(&(direction_node.v), &(direction_vectors[i]));
                    copy_vector_from(&(direction_node.parent), &(min_node.v));
                    direction_node.f = do_manhattan(s, direction_node.v) + dists[direction_node.v[0]][direction_node.v[1]];
                    realloc_node_array_if_needed(open, open_size, &open_size_max);
                    copy_node_from(&(open[open_size++]), &direction_node);
                }
        }
    }

    free(open);
    free(close);
    return total_cost;
}

void alloc_nodes(node **open, node **close) {
    allocStruct(node, open, N);
    allocStruct(node, close, N);
}

void init_start_node(node *start_node, vector s, float dists[][N]) {
    copy_vector_from((int(*)[2])(start_node->v), (int(*)[2])(s));
    copy_vector_from((int(*)[2])(start_node->parent), (int(*)[2])(s));
    start_node->f = dists[s[0]][s[1]];
}

void copy_node_from(node *n1, node *n2) {
    copy_vector_from((int(*)[2])(n1->v), (int(*)[2])(n2->v));
    copy_vector_from((int(*)[2])(n1->parent), (int(*)[2])(n2->parent));
    n1->f = n2->f;
}

void find_min_node(node *min_node, int *min_position, node open[], int open_size) {
    int i, min_f;
    min_f = open[0].f;
    *min_position = 0;
    copy_node_from(min_node, &(open[0]));
    if (open_size > 1)
        for (i=1; i<open_size; i++)
            if (open[i].f < min_f) {
                min_f = open[i].f;
                *min_position = i;
                copy_node_from(min_node, &(open[i]));
            }
}

void remove_min_node(int min_position, node *open, int open_size) {
    int i;
    for (i=min_position; i<open_size-1; i++)
        copy_node_from(&(open[i]), &(open[i+1]));
}

void realloc_node_array_if_needed(node *array, int size, int *size_max) {
    if(size+1 >= *size_max)
        array = (node *) realloc(array, (*size_max+=N)*sizeof(node));
}

int equals_vector(vector v1, vector v2) {
    return v1[0]==v2[0] && v1[1]==v2[1];
}

void delete_node(node *array, int array_size, int array_position, node direction) {
    int i;
    array_size--;
    for (i=0; i<array_size; i++)
        if (i>=array_position)
            copy_node_from(&(array[i]), &(array[i+1]));
    copy_node_from(&(array[array_size++]), &direction);
}

void set_temp_parent_and_total_cost(vector *temp_parent, node n, float *total_cost) {
    copy_vector_from(temp_parent, &(n.parent));
    *total_cost += n.f;
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
    for(i=0; i<size; i+=2)
      for(j=0; j<2; j++)
        array[i][j] = va_arg(args, int);
    va_end(args);
}

void copy_vector_from(vector *v1, vector *v2) {
    memcpy(*v1, *v2, sizeof(vector));
}

int main() {
    vector s, x, y, fast, last;
    int matrix[N][N], extensions=0;
    float distances[N][N], distances2[N][N];
    float cost1, cost2;

    srand(time(0));
    create_matrix_with_boundaries(matrix);

    puts("~~MATRIX~~");
    print_matrix(matrix, IS_INT);
    puts("");

    puts("Choices: ");
    print_options(matrix);
    input_options(s, x, y);
    set_options(matrix, s, x, y);
    print_matrix(matrix, IS_INT);
    puts("");

    if(do_manhattan(s, x) > do_manhattan(s, y)) {
        copy_vector_from(&fast, &y);
        copy_vector_from(&last, &x);
        puts("We will start with Y");
    } else {
        if(do_manhattan(s, x) < do_manhattan(s, y))
            puts("We will start with X");
        else
            puts("X and Y have equal distance, we will start with X");
        copy_vector_from(&fast, &x);
        copy_vector_from(&last, &y);
    }

    calculate_distances(distances, fast);
    print_matrix(distances, IS_FLOAT);
    cost1 = a_star(matrix, distances, s, fast, &extensions);

    print_matrix(matrix, IS_INT);
    printf("cost1: %f\n", cost1);
    printf("first extensions: %d\n", extensions);

    calculate_distances(distances2, last);

    print_matrix(distances2, IS_FLOAT);
    cost2 = a_star(matrix, distances2, fast, last, &extensions);
    print_matrix(matrix, IS_INT);
    printf("cost2: %f\n", cost2);
    printf("total cost: %f\n", cost1 + cost2);
    printf("total extensions: %d\n", extensions);

    return 0;
}

void create_matrix_with_boundaries(int matrix[][N]) {
    int p_scaled;
    int i, j;
    for (i=0; i<N; i++)
        for (j=0; j<N; j++) {
            p_scaled = (int)((rand() % MAX) + 1);
            p_scaled > P*((int)MAX+1) ? (matrix[i][j]=1) : (matrix[i][j]=0);
        }
}

void print_options(int matrix[][N]) {
    int i, j;
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            if (matrix[i][j] == 0)
                printf("(%d,%d) ", i, j);
}

void scan_for(vector v, char *letter) {
    char enter;
    printf("\ni j: ");
    scanf("%d%d", &v[0], &v[1]);
    scanf("%c", &enter);
    printf("%s(%d,%d)", letter, v[0], v[1]);
}

void input_options(vector s, vector x, vector y) {
    printf("\nPlease insert a starting point S(i,j)");
    scan_for(s, "S");
    printf("\nInsert target X(i, j), *except (%d,%d)", s[0], s[1]);
    scan_for(x, "G1");
    printf("\nInsert target Y(i, j), *except (%d,%d),(%d,%d)", s[0], s[1], x[0], x[1]);
    scan_for(y, "G2");
    printf("Status: S(%d,%d), X(%d,%d) , Y(%d,%d)\n\n", s[0], s[1], x[0], x[1], y[0], y[1]);
}

void set_options(int matrix[][N], vector s, vector x, vector y) {
    matrix[s[0]][s[1]] = G1;
    matrix[x[0]][x[1]] = G2;
    matrix[y[0]][y[1]] = T;
}

float do_manhattan(vector s, vector g) {
    return (1 * abs(g[0] - s[0])) + ((0.5) * abs(g[1] - s[1]));
}

void calculate_distances(float distances[][N], vector vec) {
    vector temp_vector;
    int i, j;
    for (i=0; i<N; i++)
        for (j=0; j<N; j++) {
            temp_vector[0] = i; temp_vector[1] = j;
            distances[i][j] = do_manhattan(temp_vector, vec);
        }
}

/*
int main() {
    node n1, n2;
    n2.v[0] = 5;
    n2.v[1] = 6;
    n2.parent[0] = 7;
    n2.parent[1] = 8;
    n2.f = 9;

    copy_node_from(&n1, &n2);

    printf("node1 %d, %d, %d, %d, %f\n", n1.v[0], n1.v[1], n1.parent[0], n1.parent[1], n1.f);
    printf("node2 %d, %d, %d, %d, %f\n", n2.v[0], n2.v[1], n2.parent[0], n2.parent[1], n2.f);

    return 0;
}
*/
