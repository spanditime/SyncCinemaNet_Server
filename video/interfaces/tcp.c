#include "errors.h"
#include "tcp.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

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
    die_pos_err(errno);
  }
  int ret = bind(sock,(struct sockaddr*)&saddr,sizeof(saddr));
  if(ret == -1) {
    die_pos_err(errno);
  }
  ret = listen(sock,1); // TODO: Найти наилучший вариант получения количества клиентов в очереди
  if(ret == -1) {
    die_pos_err(errno);
  }
  return sock;
}

int tcp_server(struct epoll_event *event, int net, int port) {
  int listen_sock, current_sock, esock;
  esock = epoll_create1(0);
  if(esock == -1) {
    die_pos_err(errno);
  }
  event->events = EPOLLIN;
  listen_sock = tcp_create(net,port,LISTEN);
  while(1) {
    current_sock = accept(listen_sock,NULL,NULL);
    event->data.fd = current_sock;
    if(epoll_ctl(esock,EPOLL_CTL_ADD,current_sock,event) == -1) {
      die_pos_err(errno);
    }
  }
}
