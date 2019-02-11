#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "rtsp.h"

#define BUF_SIZE 1024

int rtsp_server(int fd) {
  /*
  Здесь скоро будет нормальный обработчик
  */
  char *buf, *wbuf;
  buf=(char *) malloc(sizeof(char)*BUF_SIZE);
  wbuf=(char *) malloc(sizeof(char)*BUF_SIZE);
  int requestSize = read(fd,buf,BUF_SIZE);
  RtspMessage msg;

  if(requestSize == -1) {
    return 1;
  }else if(requestSize == 0){
    wbuf=strcpy(wbuf,"RTSP/1.0 400 Bad Request\0");
  }else{
    //parse
    char *tok;
    tok=strtok(buf," \n");//Method
    if(!strcmp(tok,"OPTIONS")){
      msg.method=RTSP_METHOD_OPTIONS;
    }else if(!strcmp(tok,"DESCRIBE")){
      msg.method=RTSP_METHOD_DESCRIBE;
    }else if(!strcmp(tok,"PLAY")){
      msg.method=RTSP_METHOD_PLAY;
    }else if(!strcmp(tok,"PAUSE")){
      msg.method=RTSP_METHOD_PAUSE;
    }else if(!strcmp(tok,"RECORD")){
      msg.method=RTSP_METHOD_RECORD;
    }else if(!strcmp(tok,"REDIRECT")){
      msg.method=RTSP_METHOD_REDIRECT;
    }else if(!strcmp(tok,"SETUP")){
      msg.method=RTSP_METHOD_SETUP;
    }else if(!strcmp(tok,"ANNOUNCE")){
      msg.method=RTSP_METHOD_ANNOUNCE;
    /*not supported yet
    }else if(!strcmp(tok,"GET_PARAMETER")){
      msg.method=RTSP_METHOD_GET_PARAMETER;
    }else if(!strcmp(tok,"SET_PARAMETER")){
      msg.method=RTSP_METHOD_SET_PARAMETER;
    */
    }else if(!strcmp(tok,"TEARDOWN")){
      msg.method=RTSP_METHOD_TEARDOWN;
    }else{
      msg.method=RTSP_METHOD_ERROR;
    }
    msg.content=strtok(NULL," \n");
    while(tok != 0){
      
    }
  }

  requestSize = write(fd,wbuf,strlen(wbuf));
  if(requestSize == -1) {
    return -1;
  }
  return 0;
}
