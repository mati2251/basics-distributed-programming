#include <mpi.h>
#include <stdio.h>
#define ROOT 0
#define MSG_TAG 100

int main(int argc, char **argv) {
  int tid, size;
  MPI_Status status;
  int provided;

  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &tid);

  int msg;

  if (tid == ROOT) {
    int msg = 0;
    MPI_Send(&msg, 1, MPI_INT, (tid + 1) % size, MSG_TAG, MPI_COMM_WORLD);
    printf(" Wyslalem %d do %d\n", msg, ROOT);

    MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
             &status);
    printf(" Otrzymalem %d od %d\n", msg, status.MPI_SOURCE);
  } else {
    MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
             &status);
    printf(" Otrzymalem %d od %d\n", msg, status.MPI_SOURCE);
    msg += 1;
    MPI_Send(&msg, 1, MPI_INT, (tid + 1) % size, MSG_TAG, MPI_COMM_WORLD);
    printf(" Wyslalem %d do %d\n", msg, ROOT);
  }
  MPI_Finalize();
}
