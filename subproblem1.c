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

typedef struct {
    int i, j;
    int parenti, parentj;
    float f;
} node;

int matrix[N][N];
float distances[N][N];
float distances2[N][N];
int epektaseis = 0;

void next_bool(double pr) {
    int c, v;
    int p_scaled;

    for (c = 0; c < N; c++) {
        for (v = 0; v < N; v++) {
            p_scaled = (int)((rand() % MAX) + 1);
            if (p_scaled > pr * ((int)MAX + 1))
                matrix[c][v] = 1;
            else
                matrix[c][v] = 0;
        }
    }
}

float do_manhattan(int si, int sj, int gi, int gj) {
    return (1 * abs(gi - si)) + ((0.5) * abs(gj - sj));
}

float a_star(int matrix[N][N], float ap[N][N], int si, int sj, int fasti, int fastj) {
    node *open, *close;

    open = (node *)malloc((N * N) * sizeof(node));
    if (open == NULL) exit(1);
    close = (node *)malloc((N * N) * sizeof(node));
    if (close == NULL) exit(1);
    float total_cost = 0;

    int counto = 0, countc = 0;
    node Start;

    Start.i = si;
    Start.j = sj;
    Start.parenti = si;
    Start.parentj = sj;
    Start.f = distances[si][sj];
    open[counto++] = Start;

    int z, q;
    node up, down, left, right;

    while (1) {
        if (counto == 0) break;
        node v = open[0];
        int thesh = 0;
        int minf = open[0].f;
        if (counto > 1)
            for (z = 1; z < counto; z++) {
                if (open[z].f < minf) {
                    minf = open[z].f;
                    v = open[z];
                    thesh = z;
                }
            }
        counto--;

        for (q = 0; q < counto; q++)
            if (q >= thesh)
                open[q] = open[q + 1];

        open = (node *)realloc(open, counto * sizeof(node));
        open = (node *)realloc(open, (N * N) * sizeof(node));
        close[countc++] = v;
        if (v.i == fasti && v.j == fastj) {
            int h;
            for (h = 0; h < countc; h++)
                epektaseis += 1;

            int i, j;
            int w;

            matrix[v.parenti][v.parentj] = 7;
            i = v.parenti;
            j = v.parentj;
            total_cost += v.f;
            while (1) {
                node found;
                for (w = 0; w < countc; w++)
                    if (close[w].i == i && close[w].j == j)
                        found = close[w];

                i = found.parenti;
                j = found.parentj;
                total_cost += found.f;
                if (i == si && j == sj) break;
                matrix[found.parenti][found.parentj] = 7;
            }
            break;
        } else {
            if (v.i != 0 && matrix[(v.i) - 1][v.j] != 1) {
                int o = 0, cl = 0, k, q;
                node old;
                int tho, thc;

                up.i = (v.i) - 1;
                up.j = (v.j);
                up.parenti = v.i;
                up.parentj = v.j;
                up.f = do_manhattan(si, sj, ((v.i) - 1), (v.j)) + ap[(v.i) - 1][v.j];

                for (k = 0; k < counto; k++) {
                    if (open[k].i == up.i && open[k].j == up.j) {
                        old = open[k];
                        tho = k;
                        o = 1;
                    }
                }

                for (k = 0; k < countc; k++) {
                    if (close[k].i == up.i && close[k].j == up.j) {
                        old = close[k];
                        thc = k;
                        cl = 1;
                    }
                }

                if (o == 0 && cl == 0)
                    open[counto++] = up;
                else
                if (old.f > up.f) {
                    if (o == 1) {
                        counto--;
                        for (q = 0; q < counto; q++)
                            if (q >= tho)
                                open[q] = open[q + 1];

                        open = (node *)realloc(open, counto * sizeof(node));
                        open = (node *)realloc(open, (N * N) * sizeof(node));
                        open[counto++] = up;
                    } else if (cl == 1) {
                        countc--;
                        for (q = 0; q < countc; q++)
                            if (q >= thc)
                                close[q] = close[q + 1];

                        close = (node *)realloc(close, countc * sizeof(node));
                        close = (node *)realloc(close, (N * N) * sizeof(node));
                        open[counto++] = up;
                    }
                }
            }

            if (v.i != N && matrix[(v.i) + 1][v.j] != 1) {
                int o = 0, cl = 0, k, q;
                int tho, thc;
                node old;

                down.i = (v.i) + 1;
                down.j = (v.j);
                down.parenti = v.i;
                down.parentj = v.j;
                down.f = do_manhattan(si, sj, ((v.i) + 1), (v.j)) + ap[(v.i) + 1][v.j];

                for (k = 0; k < counto; k++) {
                    if (open[k].i == down.i && open[k].j == down.j) {
                        old = open[k];
                        tho = k;
                        o = 1;
                    }
                }

                for (k = 0; k < countc; k++) {
                    if (close[k].i == down.i && close[k].j == down.j) {
                        old = close[k];
                        thc = k;
                        cl = 1;
                    }
                }

                if (o == 0 && cl == 0)
                    open[counto++] = down;
                else
                if (old.f > down.f) {
                    if (o == 1) {
                        counto--;
                        for (q = 0; q < counto; q++)
                            if (q >= tho)
                                open[q] = open[q + 1];

                        open = (node *)realloc(open, counto * sizeof(node));
                        open = (node *)realloc(open, (N * N) * sizeof(node));
                        open[counto++] = down;
                    } else if (cl == 1) {
                        countc--;
                        for (q = 0; q < countc; q++)
                            if (q >= thc)
                                close[q] = close[q + 1];

                        close = (node *)realloc(close, countc * sizeof(node));
                        close = (node *)realloc(close, (N * N) * sizeof(node));
                        open[counto] = down;
                        counto++;
                    }
                }
            }

            if (v.j != 0 && matrix[v.i][(v.j) - 1] != 1) {
                int o = 0, cl = 0, k, q;
                int tho, thc;
                node old;
                left.i = (v.i);
                left.j = (v.j) - 1;
                left.parenti = v.i;
                left.parentj = v.j;
                left.f = do_manhattan(si, sj, (v.i), ((v.j) - 1)) + ap[v.i][(v.j) - 1];
                for (k = 0; k < counto; k++) {
                    if (open[k].i == left.i && open[k].j == left.j) {
                        old = open[k];
                        tho = k;
                        o = 1;
                    }
                }
                for (k = 0; k < countc; k++) {
                    if (close[k].i == left.i && close[k].j == left.j) {
                        old = close[k];
                        thc = k;
                        cl = 1;
                    }
                }
                if (o == 0 && cl == 0) {
                    open[counto++] = left;
                } else {
                    if (old.f > left.f) {
                        if (o == 1) {
                            counto--;
                            for (q = 0; q < counto; q++)
                                if (q >= tho)
                                    open[q] = open[q + 1];
                            open = (node *)realloc(open, counto * sizeof(node));
                            open = (node *)realloc(open, (N * N) * sizeof(node));
                            open[counto++] = left;
                        } else if (cl == 1) {
                            countc--;
                            for (q = 0; q < countc; q++)
                                if (q >= thc)
                                    close[q] = close[q + 1];
                            close = (node *)realloc(close, countc * sizeof(node));
                            close = (node *)realloc(close, (N * N) * sizeof(node));
                            open[counto++] = left;
                        }
                    }
                }
            }
            if (v.j != N && matrix[v.i][(v.j) + 1] != 1) {
                int o = 0, cl = 0, k, q;
                int tho, thc;
                node old;
                right.i = (v.i);
                right.j = (v.j) + 1;
                right.parenti = v.i;
                right.parentj = v.j;
                right.f = do_manhattan(si, sj, (v.i), ((v.j) + 1)) + ap[v.i][(v.j) + 1];
                for (k = 0; k < counto; k++) {
                    if (open[k].i == right.i && open[k].j == right.j) {
                        old = open[k];
                        tho = k;
                        o = 1;
                    }
                }
                for (k = 0; k < countc; k++) {
                    if (close[k].i == right.i && close[k].j == right.j) {
                        old = close[k];
                        thc = k;
                        cl = 1;
                    }
                }
                if (o == 0 && cl == 0) {
                    open[counto++] = right;
                } else {
                    if (old.f > right.f) {
                        if (o == 1) {
                            counto--;
                            for (q = 0; q < counto; q++)
                                if (q >= tho)
                                    open[q] = open[q + 1];
                            open = (node *)realloc(open, counto * sizeof(node));
                            open = (node *)realloc(open, (N * N) * sizeof(node));
                            open[counto++] = right;
                        } else if (cl == 1) {
                            countc--;
                            for (q = 0; q < countc; q++)
                                if (q >= thc)
                                    close[q] = close[q + 1];
                            close = (node *)realloc(close, countc * sizeof(node));
                            close = (node *)realloc(close, (N * N) * sizeof(node));
                            open[counto++] = right;
                        }
                    }
                }
            }
        }
    }
    return total_cost;
}

int main() {
    srand(time(0));
    next_bool(p);
    char enter;
    int i, j, k, l;
    int si, sj;
    int xi, xj;
    int yi, yj;
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

    for (k = 0; k < N; k++)
        for (l = 0; l < N; l++)
            distances[k][l] = do_manhattan(k, l, fasti, fastj);

    print_matrix(distances, IS_FLOAT);
    cost1 = a_star(matrix, distances, si, sj, fasti, fastj);
    print_matrix(matrix, IS_INT);
    printf("cost1: %f\n", cost1);
    printf("epektaseis 1h fora: %d\n", epektaseis);
    for (k = 0; k < N; k++)
        for (l = 0; l < N; l++)
            distances2[k][l] = do_manhattan(k, l, lasti, lastj);
    print_matrix(distances2, IS_FLOAT);
    cost2 = a_star(matrix, distances2, fasti, fastj, lasti, lastj);
    print_matrix(matrix, IS_INT);
    printf("cost2: %f\n", cost2);
    printf("total cost: %f\n", cost1 + cost2);
    printf("sunolikes epektaseis pou eginan: %d\n", epektaseis);
    return 0;
}
