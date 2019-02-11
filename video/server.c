#include "interfaces/tcp.h"
#include "rtsp/rtsp.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <stdlib.h>

struct start_data {
  struct epoll_event *event;
  char* ip;
  int port;
  int efd;
};

void* start_epoll(void* param);
void* start_listen(void* param);

int main(int argc, char** argv) {
  struct start_data data;
  struct epoll_event main_event;
  char* ip;
  int port;
  if(argc == 2) {
    ip = NULL;
    port = atoi(argv[1]);
  } else if(argc == 3) {
    ip = argv[1];
    port = atoi(argv[2]);
  } else {
    puts("Invalid list of arguments");
    return 1;
  }
  data.event = &main_event;
  data.ip = ip;
  data.efd = -1;
  data.port = port;
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_create(&tid1,&attr,start_epoll,&data);
  pthread_create(&tid2,&attr,start_listen,&data);
  pthread_join(tid2,NULL);
  pthread_join(tid1,NULL);
}

void* start_epoll(void* param) {
  struct start_data* data = (struct start_data*) param;
  tcp_server(data->event,&(data->efd),data->ip,data->port);
  return NULL;
}

void* start_listen(void* param) {
  struct start_data* data = (struct start_data*) param;
  tcp_listen(&(data->efd),rtsp_server);
  return NULL;
}
