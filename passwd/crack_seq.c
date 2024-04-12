#include <crypt.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 4 
char *stro = "ddoo3WocSpthU";

void next_permutation(char *word, char* salt, int position) {
  for (int i = 0; i < 26; i++) {
    word[position] = 'a' + i;
    if (position != SIZE - 1)
      next_permutation(word, salt, position + 1);
    else {
      char *x = crypt(word, salt);
      if ((strcmp(x, stro)) == 0) {
        printf("Udalo sie: %s %s %s\n", word, x, stro);
        exit(0);
      }
    }
  }
}

int main(int argc, char **argv) {
  char cmp[SIZE + 1] = {0};

  char salt[3] = {0};
  salt[0] = stro[0];
  salt[1] = stro[1];
  next_permutation(cmp, salt, 0);

  return 0;
}
