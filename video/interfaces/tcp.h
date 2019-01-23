#ifndef SERVER_TCP
  #define SERVER_TCP 1
  #define LISTEN 10000
  #ifndef NULL
    #define NULL (void*)0
  #endif
  #include <sys/epoll.h>
  enum {LOCAL, INET};
  int tcp_create(int net, int port, int size);
  int tcp_server(struct epoll_event *event, int net, int port);
#endif
