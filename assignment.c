#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // initialize random number generator
    srand(time(NULL));
    int minrand = 1;
    int maxrand = 100;

    // Check the number of arguments
    if (argc != 3) {
        printf(
            "Incorrect usage. You provided %d arguments. The correct number of "
            "arguments is 2\n",
            argc - 1);
        return 1;
    }

    // Validate arguments: check if they are positive integers
    for (int i = 1; i < argc; ++i) {
        for (int j = 0; argv[i][j] != '\0'; ++j) {
            if (!isdigit(argv[i][j])) {
                printf(
                    "Incorrect usage. The parameters you provided are not "
                    "positive integers\n");
                return 1;
            }
        }
        if (atoi(argv[i]) <= 0) {
            printf(
                "Incorrect usage. The parameters you provided are not positive "
                "integers\n");
            return 1;
        }
    }

    // Parse dimensions
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    // Dynamically allocate the matrix
    int **matrix = malloc(rows * sizeof(int *));
    if (!matrix) {
        perror("Failed to allocate memory for rows");
        return 1;
    }

    for (int i = 0; i < rows; ++i) {
        matrix[i] = malloc(cols * sizeof(int));
        if (!matrix[i]) {
            perror("Failed to allocate memory for columns");
            for (int j = 0; j < i; ++j) {
                free(matrix[j]);
            }
            free(matrix);
            return 1;
        }
    }

    // Fill the matrix with random values
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = minrand + rand() % (maxrand - minrand + 1);
        }
    }

    // Write the matrix to "matrix.txt"
    int fd = open("matrix.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open matrix.txt");
        for (int i = 0; i < rows; ++i) {
            free(matrix[i]);
        }
        free(matrix);
        return 1;
    }

    char buffer[256];
    for (int i = 0; i < rows; ++i) {
        int len = 0;
        for (int j = 0; j < cols; ++j) {
            len += snprintf(buffer + len, sizeof(buffer) - len, "%d",
                            matrix[i][j]);
            if (j < cols - 1) {
                len += snprintf(buffer + len, sizeof(buffer) - len, " ");
            }
        }
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
                 "\n");
        write(fd, buffer, strlen(buffer));
    }

    close(fd);

    // Free dynamically allocated memory
    for (int i = 0; i < rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
