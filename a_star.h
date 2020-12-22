#include "global.h"
#include "general.h"

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
void set_temp_parent_and_total_cost(vector *, node, float *);
void delete_node(node *, int, int, node);
float do_manhattan(vector, vector);

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

float do_manhattan(vector s, vector g) {
    return (1 * abs(g[0] - s[0])) + ((0.5) * abs(g[1] - s[1]));
}