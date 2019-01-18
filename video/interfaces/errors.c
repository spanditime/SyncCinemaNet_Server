#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern int errno;
char *str_hand[] = {"This function is not ready yet"};

void die_err(int err) {
  printf("%s: %s\n","Die",str_hand[err]);
  exit(1);
}
void die_pos_err(int err) {
  errno = err;
  perror("Die");
  exit(err);
}
