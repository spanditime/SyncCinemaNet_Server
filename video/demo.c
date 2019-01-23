#include "interfaces/tcp.h"
#include <stdio.h>
#include <sys/epoll.h>

struct epoll_event main_event;

int main() {
  //printf("%d\n",tcp_create(LOCAL, 8000, 1));
  tcp_server(&main_event,LOCAL, 8000);
}
