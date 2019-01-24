#include "errors.h"
#include "tcp.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

extern int errno;

int tcp_create(int net, int port, int size) {
  struct hostent host;
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	if (net == LOCAL) {
		saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
    die_err(SE_DEV);
  }
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock == -1) {
    perror("socket");
    return -1;
    //die_pos_err(errno);
  }
  int ret = bind(sock,(struct sockaddr*)&saddr,sizeof(saddr));
  if(ret == -1) {
    perror("bind");
    //return -1;
    die_pos_err(errno);
  }
  ret = listen(sock,LISTEN); // TODO: Найти наилучший вариант получения количества клиентов в очереди
  if(ret == -1) {
    perror("listen");
    return -1;
    //die_pos_err(errno);
  }
  return sock;
}

int tcp_server(struct epoll_event *event,int* efd, int net, int port) {
  int listen_sock, current_sock, esock;
  esock = epoll_create1(0);
  if(esock == -1) {
    //die_pos_err(errno);
    perror("epoll_create1");
    return -1;
  }
  *efd = esock;
  event->events = EPOLLIN;
  listen_sock = tcp_create(net,port,LISTEN);
  while(1) {
    current_sock = accept(listen_sock,NULL,NULL);
    event->data.fd = current_sock;
    if(epoll_ctl(esock,EPOLL_CTL_ADD,current_sock,event) == -1) {
      //die_pos_err(errno);
      perror("epoll_create1");
      return -1;
    }
  }
}

int tcp_listen(int* efd, int (*callback)(int)) {
  int fd, ret;
  struct epoll_event evs[1];
  while(1) {
    if (*efd == -1) {
      //perror("tcp_listen");
      continue;
    }
    ret = epoll_wait(*efd,evs,1,LISTEN);
    if(ret > 0) {
      fd = evs[0].data.fd;
      ret = (*callback)(fd);
      if(ret != 0) {
        die_err(ret);
      }
    } else if(ret == -1) {
      perror("epoll_wait");
      return -1;
      //die_pos_err(errno);
    }
  }
}
