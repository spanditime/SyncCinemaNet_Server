#include "interfaces/tcp.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>

struct epoll_event main_event;
#define BUF_SIZE 256
void* start1(void* param);
void* start2(void* param);
int echo_server(int fd);
int efd = -1;
char* ip;

int main(int argc, char** argv) {
  if(argc != 2) {
    ip = NULL;
  } else {
    ip = argv[1];
  }
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_create(&tid1,&attr,start1,NULL);
  pthread_create(&tid2,&attr,start2,NULL);
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
}

int echo_server(int fd) {
  char buf[BUF_SIZE], wbuf[BUF_SIZE];
  int ret = read(fd,buf,BUF_SIZE);
  if(ret == -1) {
    return 1;
  }
  ret = write(fd,buf,ret);
  if(ret == -1) {
    return -1;
  }
  return 0;
}

void* start1(void* param) {
  tcp_server(&main_event,&efd,ip,8000);
  return NULL;
}

void* start2(void* param) {
  tcp_listen(&efd,echo_server);
  return NULL;
}
