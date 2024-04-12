#include <crypt.h>
#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 5
char *stro = "aaSPfLTmjh3fU";
int rank;

void next_permutation(char *word, char *salt, int position) {
  for (int i = 0; i < 26; i++) {
    word[position] = 'a' + i;
    if (position != SIZE - 1)
      next_permutation(word, salt, position + 1);
    else {
      char *x = crypt(word, salt);
      if ((strcmp(x, stro)) == 0) {
        if (rank != 0) {
          MPI_Send(word, SIZE, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
          MPI_Finalize();
          exit(0);
        } else {
          printf("Found: %s\n", word);
          MPI_Finalize();
        }
      }
    }
  }
}

pthread_t watek_msg_handler;
void *msg_handler(void *param) {
  char word[SIZE + 1] = {0};
  MPI_Status status;
  MPI_Recv(word, SIZE, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
           &status);
  printf("Found: %s\n", word);
  MPI_Finalize();
  return 0;
}

int main(int argc, char **argv) {
  int size;

  int provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  if (provided != MPI_THREAD_MULTIPLE) {
    perror(
        "Parallel environment does not provide the required thread support\n");
    MPI_Finalize();
    exit(0);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0)
    pthread_create(&watek_msg_handler, 0, msg_handler, 0);

  char cmp[SIZE + 1] = {0};

  char salt[3] = {0};
  salt[0] = stro[0];
  salt[1] = stro[1];

  cmp[0] = 'a' + rank;

  next_permutation(cmp, salt, 1);
  if (rank == 0) {
    pthread_join(watek_msg_handler, 0);
    printf("join\n");
  }

  MPI_Finalize();
}
