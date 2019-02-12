#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "rtsp.h"
#include "interfaces/errors.h"

#define BUF_SIZE 1024

int rtsp_server(int fd) {
  die_err(SE_DEV);
  return -1;

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
    strcpy(wbuf,"RTSP/1.0 400 Bad Request\0");
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
    }else if(!strcmp(tok,"GET_PARAMETER")){
      msg.method=RTSP_METHOD_GET_PARAMETER;
    }else if(!strcmp(tok,"SET_PARAMETER")){
      msg.method=RTSP_METHOD_SET_PARAMETER;
    }else if(!strcmp(tok,"TEARDOWN")){
      msg.method=RTSP_METHOD_TEARDOWN;
    }else{
      msg.method=RTSP_METHOD_ERROR;
    }
    msg.URI=strtok(NULL," \n");
    strtok(NULL," /");//пропускаем RTSP
    msg.rtspVerMajor=atoi(strtok(NULL,"/."));
    msg.rtspVerMinor=atoi(strtok(NULL,". \n"));
    msg.cseq=0;
    msg.fieldsCount=0;

    msg.fields=(struct Field *) malloc(sizeof(struct Field));
    tok=strtok(NULL,": \n");//cтрока
    while(tok != NULL){
      //парсинг пары заголовок: значение
      msg.fields=(struct Field *) realloc((void *)msg.fields,sizeof(struct Field)*(msg.fieldsCount+=1));
      msg.fields[msg.fieldsCount-1].header=tok;
      tok=strtok(NULL,": \n");
      if(tok != NULL){
        msg.fields[msg.fieldsCount-1].value=(char *) malloc(sizeof(char)*strlen(tok));
        strcpy(msg.fields[msg.fieldsCount-1].value,tok);
      }else{
        strcpy(wbuf,"RSTP/1.0 451 Parameter Not Understood");
        free(buf);
        tok=NULL;
        goto Write;
      }
      //проверка на Content-Length
      if(!strcmp(msg.fields[msg.fieldsCount-1].header,"Content-Length")){
        // +2 для безопасности, для \0
        msg.content=strtok(NULL,"\0");
        free(buf);
        tok=NULL;
        strcpy(wbuf,"DONE");
        goto Write;
      }
      tok=strtok(NULL,": \n");//cтрока
    }

  }
  
Write:
  requestSize = write(fd,wbuf,strlen(wbuf));
  if(requestSize == -1) {
    return -1;
  }
  return 0;
}
