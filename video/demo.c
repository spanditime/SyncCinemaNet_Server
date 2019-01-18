#include "interfaces/tcp.h"
#include <stdio.h>

int main() {
  printf("%d\n",tcp_create(LOCAL, 8000, 1));
}
