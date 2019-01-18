#ifndef SERVER_TCP
  #define SERVER_TCP 1
  enum {LOCAL, INET};
  int tcp_create(int net, int port, int size);
#endif
