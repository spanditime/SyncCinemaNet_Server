#include <unistd.h>

#define BUF_SIZE 1024

int rstp_server(int fd) {
  /*
  Здесь скоро будет нормальный обработчик
  */
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
