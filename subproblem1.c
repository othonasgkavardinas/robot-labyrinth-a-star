#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define N     5
#define p     0.7
#define MAX   100

#define IS_INT   1
#define IS_FLOAT 0

#define print_matrix(matrix, type) ({ \
    int i, j; \
    for (i = 0; i < N; i++, puts("")) \
        for (j = 0; j < N; j++) \
            printf(type==IS_INT?"%d ":"%.2f ", matrix[i][j]); \
})

void init_start_node(node, int, int);
void realloc_node_array(node *, int);

typedef struct {
    int i, j;
    int parenti, parentj;
    float f;
} node;

int matrix[N][N];
float distances[N][N];
float distances2[N][N];
int extensions = 0;

void next_bool(double pr) {
    int i, j;
    int p_scaled;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            p_scaled = (int)((rand() % MAX) + 1);
            if (p_scaled > pr * ((int)MAX + 1))
                matrix[i][j] = 1;
            else
                matrix[i][j] = 0;
        }
    }
}

float do_manhattan(int si, int sj, int gi, int gj) {
    return (1 * abs(gi - si)) + ((0.5) * abs(gj - sj));
}

float a_star(int matrix[N][N], float ap[N][N], int si, int sj, int fasti, int fastj) {
    node *open, *close;
    int open_size, close_size, position, minf, i, j, open_flag, close_flag, open_position, close_position;
    float total_cost;
    node start_node, v_node, up, down, left, right, found, old, temp_parenti, temp_parentj;

    open_size = 0;
    close_size = 0;
    open = (node *)malloc((N * N) * sizeof(node));
    if (open == NULL) exit(1);
    close = (node *)malloc((N * N) * sizeof(node));
    if (close == NULL) exit(1);

    init_start_node(start_node, si, sj);
    open[open_size++] = start_node;

    total_cost = 0;

    while (1) {
        if (open_size == 0) break;
        v_node = open[0];
        position = 0;
        minf = open[0].f;
        if (open_size > 1)
            for (i=1; i<open_size; i++) {
                if (open[i].f < minf) {
                    minf = open[i].f;
                    v_node = open[i];
                    position = i;
                }
            }
        open_size--;

        for (i=0; i<open_size; i++)
            if (i >= position)
                open[i] = open[i + 1];

        realloc_node_array(open, open_size);
        close[close_size++] = v_node;
        if (v_node.i == fasti && v_node.j == fastj) {
            extensions = close_size;

            matrix[v_node.parenti][v_node.parentj] = 7;
            temp_parenti = v_node.parenti;
            temp_parentj = v_node.parentj;
            total_cost += v_node.f;
            while (1) {
                for (i = 0; i < close_size; i++)
                    if (close[i].i == temp_parenti && close[i].j == temp_parentj)
                        found = close[i];

                temp_parenti = found.parenti;
                temp_parentj = found.parentj;
                total_cost += found.f;
                if (temp_parenti == si && temp_parentj == sj) break;
                matrix[found.parenti][found.parentj] = 7;
            }
            break;
        } else {
            if (v_node.i != 0 && matrix[(v_node.i) - 1][v_node.j] != 1) {
                open_flag = 0;
                close_flag = 0;

                up.i = (v_node.i) - 1;
                up.j = (v_node.j);
                up.parenti = v_node.i;
                up.parentj = v_node.j;
                up.f = do_manhattan(si, sj, ((v_node.i) - 1), (v_node.j)) + ap[(v_node.i) - 1][v_node.j];

                old = set_node(open, open_size, open_position, open_flag, up);
                old = set_node(close, close_size, close_position, close_flag, up);

                if (open_flag == 0 && close_flag == 0)
                    open[open_size++] = up;
                else if (old.f > up.f) {
                    if (open_flag == 1)
                      delete_element(open, open_size, open_position, up);
                    else if (close_flag == 1)
                      delete_element(close, close_size, close_position, up);
                }
            }

            if (v_node.i != N && matrix[(v_node.i) + 1][v_node.j] != 1) {
                open_flag = 0;
                close_flag = 0;

                down.i = (v_node.i) + 1;
                down.j = (v_node.j);
                down.parenti = v_node.i;
                down.parentj = v_node.j;
                down.f = do_manhattan(si, sj, ((v_node.i) + 1), (v_node.j)) + ap[(v_node.i) + 1][v_node.j];

                old = set_node(open, open_size, open_position, open_flag, down);
                old = set_node(close, close_size, close_position, close_flag, down);

                if (open_flag == 0 && close_flag == 0)
                    open[open_size++] = down;
                else
                if (old.f > down.f) {
                    if (open_flag == 1)
                      delete_element(open, open_size, open_position, down);
                    else if (close_flag == 1)
                      delete_element(close, close_size, close_position, down);
                }
            }

            if (v_node.j != 0 && matrix[v_node.i][(v_node.j) - 1] != 1) {
                open_flag = 0;
                close_flag = 0;

                left.i = (v_node.i);
                left.j = (v_node.j) - 1;
                left.parenti = v_node.i;
                left.parentj = v_node.j;
                left.f = do_manhattan(si, sj, (v_node.i), ((v_node.j) - 1)) + ap[v_node.i][(v_node.j) - 1];

                old = set_node(open, open_size, open_position, open_flag, left);
                old = set_node(close, close_size, close_position, close_flag, left);

                if (open_flag == 0 && close_flag == 0)
                    open[open_size++] = left;
                else {
                    if (old.f > left.f) {
                      if (open_flag == 1)
                        delete_element(open, open_size, open_position, left);
                      else if (close_flag == 1)
                        delete_element(close, close_size, close_position, left);
                    }
                }
            }
            if (v_node.j != N && matrix[v_node.i][(v_node.j) + 1] != 1) {
                open_flag = 0;
                close_flag = 0;

                right.i = (v_node.i);
                right.j = (v_node.j) + 1;
                right.parenti = v_node.i;
                right.parentj = v_node.j;
                right.f = do_manhattan(si, sj, (v_node.i), ((v_node.j) + 1)) + ap[v_node.i][(v_node.j) + 1];

                old = set_node(open, open_size, open_position, open_flag, right);
                old = set_node(close, close_size, close_position, close_flag, right);

                if (open_flag == 0 && close_flag == 0)
                    open[open_size++] = right;
                else {
                    if (old.f > right.f) {
                        if (open_flag == 1)
                          delete_element(open, open_size, open_position, right);
                        else if (close_flag == 1)
                          delete_element(close, close_size, close_position, right);
                    }
                }
            }
        }
    }
    return total_cost;
}

void init_start_node(node start_node, int si, int sj) {
    start_node.i = si;
    start_node.j = sj;
    start_node.parenti = si;
    start_node.parentj = sj;
    start_node.f = distances[si][sj];
}

node set_node(node *array, int array_size, int array_position, int array_flag, node direction) {
    int i;
    node old;
    for (i=0; i<uode_size; i++)
        if (array[i].i == direction.i && array[i].j == direction.j) {
            old = array[i];
            array_position = i;
            array_flag = 1;
        }
    return old;
}

void delete_node(node *array, int array_size, int array_position, node direction) {
    int i;
    array_size--;
    for (i = 0; i < array_size; i++)
        if (i >= array_position)
            array[i] = array[i + 1];
    realloc_node_array(array, array_size);
    array[array_size++] = direction;
}

void realloc_node_array(node *array, size) {
  array = (node *)realloc(array , size* sizeof(node));
  array = (node *)realloc(array , (N * N) * sizeof(node));
}

int main() {
    srand(time(0));
    next_bool(p);
    char enter;
    int i, j, k, l, si, sj, xi, xj, yi, yj;
    float cost1, cost2;

    printf("~~MATRIX~~\n");
    print_matrix(matrix, IS_INT);
    printf("\nChoices: \n");
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (matrix[i][j] == 0)
                printf("(%d,%d) ", i, j);
    printf("\nPlease insert a starting point S(i,j)\n"
          "i j:");
    scanf("%d%d", &si, &sj);
    scanf("%c", &enter);
    printf("S(%d,%d)", si, sj);
    printf("\nInsert target X(i, j), *except (%d,%d)\n", si, sj);
    printf("i j:");
    scanf("%d%d", &xi, &xj);
    scanf("%c", &enter);
    printf("G1(%d,%d)", xi, xj);
    printf("\nInsert target Y(i, j), *except (%d,%d),(%d,%d)\n", si, sj, xi, xj);
    printf("i j:");
    scanf("%d%d", &yi, &yj);
    scanf("%c", &enter);
    printf("Y(%d,%d)", yi, yj);
    printf("Status: S(%d,%d), X(%d,%d) , Y(%d,%d)\n\n", si, sj, xi, xj, yi, yj);
    matrix[si][sj] = 2;
    matrix[xi][xj] = 3;
    matrix[yi][yj] = 4;
    print_matrix(matrix, IS_INT);
    puts("");
    int fasti = 0, fastj = 0, lasti = 0, lastj = 0;
    if (do_manhattan(si, sj, xi, xj) < do_manhattan(si, sj, yi, yj)) {
        fasti = xi;
        fastj = xj;
        lasti = yi;
        lastj = yj;
        printf("We will start with X\n");
    } else if (do_manhattan(si, sj, xi, xj) > do_manhattan(si, sj, yi, yj)) {
        fasti = yi;
        fastj = yj;
        lasti = xi;
        lastj = xj;
        printf("We will start with Y\n");
    } else {
        printf("X and Y have equal distance, we will start with X\n");
        fasti = xi;
        fastj = xj;
        lasti = yi;
        lastj = yj;
    }

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            distances[i][j] = do_manhattan(i, j, fasti, fastj);

    print_matrix(distances, IS_FLOAT);
    cost1 = a_star(matrix, distances, si, sj, fasti, fastj);
    print_matrix(matrix, IS_INT);
    printf("cost1: %f\n", cost1);
    printf("epektaseis 1h fora: %d\n", extensions);
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            distances2[i][j] = do_manhattan(i, j, lasti, lastj);
    print_matrix(distances2, IS_FLOAT);
    cost2 = a_star(matrix, distances2, fasti, fastj, lasti, lastj);
    print_matrix(matrix, IS_INT);
    printf("cost2: %f\n", cost2);
    printf("total cost: %f\n", cost1 + cost2);
    printf("sunolikes epektaseis pou eginan: %d\n", extensions);
    return 0;
}
