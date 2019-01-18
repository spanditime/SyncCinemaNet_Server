#include "errors.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

extern int errno;
enum {LOCAL, INET};

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
