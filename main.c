#include "a_star.h"

void create_matrix_with_boundaries(int [][N]);
void print_options(int [][N]);
void scan_for(vector, char *);
void input_options(vector, vector, vector);
void set_options(int [][N], vector, vector, vector);
void calculate_distances(float [][N], vector);

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

void calculate_distances(float distances[][N], vector vec) {
    vector temp_vector;
    int i, j;
    for (i=0; i<N; i++)
        for (j=0; j<N; j++) {
            temp_vector[0] = i; temp_vector[1] = j;
            distances[i][j] = do_manhattan(temp_vector, vec);
        }
}