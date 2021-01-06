#include "global.h"
#include "general.c"

typedef struct {
    vector v;
    vector parent;
    float f;
} node;

float a_star(int [][N], float [][N], vector, vector, int *);
void alloc_nodes(node **, node **);
void init_start_node(node *, vector, float [][N]);
void copy_node_from(node *, node *);
void remove_node_from_list(node *, int);
void realloc_node_array(node **, int, int *);
void set_temp_parent_and_total_cost(vector *, node, float *);
float do_manhattan(vector, vector);
char vector_exists_in_set(vector, node *, int);
int cmp(const void *, const void *);

float a_star(int matrix[][N], float dists[][N], vector s, vector goal, int *extensions) {
    int open_size=0, open_size_max=N;
    int close_size=0, close_size_max=N;
    node *open, *close;
    node min_node, start_node, direction_node, found, old;
    float total_cost = 0;
    vector temp_parent;
    int i;
    int conditions_size = 4, conditions[conditions_size];
    vector direction_vectors[conditions_size];

    alloc_nodes(&open, &close);
    init_start_node(&(open[open_size++]), s, dists);

    while (open_size!=0) {
        copy_node_from(&min_node, &(open[0]));
        remove_node_from_list(open, open_size--);
        
        if(vector_exists_in_set(min_node.v, close, close_size))
            continue;

        if (equals_vector(min_node.v, goal)) {
            *extensions = close_size;
            matrix[min_node.parent[0]][min_node.parent[1]] = PASSED;
            set_temp_parent_and_total_cost(&temp_parent, min_node, &total_cost);
            while (TRUE) {
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
        }

        set_int_array(conditions, conditions_size,
            min_node.v[0] != 0 && matrix[(min_node.v[0]) - 1][min_node.v[1]] == 0,
            min_node.v[0] != N && matrix[(min_node.v[0]) + 1][min_node.v[1]] == 0,
            min_node.v[1] != 0 && matrix[min_node.v[0]][(min_node.v[1]) - 1] == 0,
            min_node.v[1] != N && matrix[min_node.v[0]][(min_node.v[1]) + 1] == 0);
        set_vector_array_from_pairs(direction_vectors, conditions_size,
            min_node.v[0]-1, min_node.v[1],
            min_node.v[0]+1, min_node.v[1],
            min_node.v[0], min_node.v[1]-1,
            min_node.v[0], min_node.v[1]+1);

        for(i=0; i<conditions_size; i++)
            if(conditions[i]) {
                copy_vector_from(&(direction_node.v), &(direction_vectors[i]));
                copy_vector_from(&(direction_node.parent), &(min_node.v));
                direction_node.f = do_manhattan(s, direction_node.v) + dists[direction_node.v[0]][direction_node.v[1]];
                realloc_node_array(&open, open_size, &open_size_max);
                copy_node_from(&(open[open_size++]), &direction_node);
            }
        qsort(open, open_size, sizeof(node), cmp);

        realloc_node_array(&close, close_size, &close_size_max);
        copy_node_from(&close[close_size++], &min_node);
    }

    free(open);
    free(close);
    return total_cost;
}

void alloc_nodes(node **open, node **close) {
    alloc_struct(node, open, N);
    alloc_struct(node, close, N);
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

void remove_node_from_list(node *open, int open_size) {
    int i;
    for (i=0; i<open_size-1; i++)
        copy_node_from(&(open[i]), &(open[i+1]));
}

void realloc_node_array(node **array, int size, int *size_max) {
    if(size+1 >= *size_max) {
        *size_max += N;
        *array = (node *) realloc(*array, (*size_max)*sizeof(node));
    }
}

void set_temp_parent_and_total_cost(vector *temp_parent, node n, float *total_cost) {
    copy_vector_from(temp_parent, &(n.parent));
    *total_cost += n.f;
}

float do_manhattan(vector s, vector g) {
    return (1 * abs(g[0] - s[0])) + ((0.5) * abs(g[1] - s[1]));
}

char vector_exists_in_set(vector v, node *set, int size) {
        int i;
        for(i=0; i<size; i++)
            if(equals_vector(v, set[i].v))
                return 1;
        return 0;
}

int cmp(const void *a, const void *b) {
    return ((node *)a)->f - ((node *)b)->f;
}