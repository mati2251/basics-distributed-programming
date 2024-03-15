#include <fcntl.h>
#include <limits.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define SORT 1
#define END 2


// Uneffectively sorts the numbers in the file for academic purposes
int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int size = 0, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  size = size - 1;
  int* numbers = malloc(size * sizeof(int));
  int* sorted = malloc(size + 1 * sizeof(int));
  int min = INT_MIN;
  int tmp = INT_MIN;
  int i;
  int end = 0;
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc < 2) {
    MPI_Finalize();
    printf("Usage: %s file-name\n", argv[0]);
    exit(0);
  }

  if (rank == 0) {
    FILE *f;
    int i;
    f = fopen(argv[1], "r");
    for (i = 0; i < size; i++)
      fscanf(f, "%d", &(numbers[i]));
    for (i = 0; i < size; i++)
      printf("%d ", (numbers[i]));

    printf("\n------------\n");

    for (i = 0; i < size; i++) {
      MPI_Send(&numbers[i], 1, MPI_INT, 1, SORT, MPI_COMM_WORLD);
    }
    int dummy = -1;
    MPI_Send(&dummy, 1, MPI_INT, 1, END, MPI_COMM_WORLD); 

    for (i = 1; i <= size; i++) {
      MPI_Recv(&sorted[i - 1], 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
    }

    for (i = 0; i < size; i++) {
      printf("%d ", sorted[i]);
    }
    printf("\n");

  } else { 
    while (!end) {
      MPI_Recv(&tmp, 1, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
      if (status.MPI_TAG == END) {
        end = 1;
        if (rank < size)
          MPI_Send(&min, 1, MPI_INT, rank + 1, END, MPI_COMM_WORLD);
        MPI_Send(&min, 1, MPI_INT, 0, SORT, MPI_COMM_WORLD);
      } else {
        if (min == INT_MIN) {
          min = tmp;
        } else if (tmp <= min) {
          MPI_Send(&min, 1, MPI_INT, rank + 1, SORT, MPI_COMM_WORLD);
          min = tmp;
        } else if (tmp > min) {
          MPI_Send(&tmp, 1, MPI_INT, rank + 1, SORT, MPI_COMM_WORLD);
        }
      }
    }
  }

  MPI_Finalize();
}
