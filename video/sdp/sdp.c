#include <string.h>
#include <stdlib.h>
#include <stdio.h>//sprintf

#include "sdp.h"
#include "interfaces/errors.h"

int strcat_connections(struct SdpConnectionData *connections,int connectionsCount,char *str);
int strcat_bandwidths(struct SdpBandwidth *bandwidths,int bandwidthsCount,char *str);
int strcat_attributes(struct SdpAttribute *attributes,int attributesCount,char *str);

int parse_sdp(struct SdpContent *sdp,char *str){
  //die_err(SE_DEV);
  //return -1;
  /*
    parser
  */
}

/**
 * /brief
 *
 * /todo make SdpAttributes checking
 * /todo update return error codes
 *
 * /param[out] str Output SDP string
 * /param[in] sdp Input SDP structure for reparse into string str
 * /return Error code or 0 if its done succeful
*/
int sdpstr(char *str,struct SdpContent *sdp){
  //die_err(SE_DEV);
  //return -1;
  /*
    reparser
  */
  if(sdp == NULL || str == NULL)
    return -1;

  if(sdp->version)
    return sdp->version;

  /*
    Checking compulsory SDP fields
  */
  if(sdp->origin.username == NULL)
    return -1;//update return value to error code(enum SdpErrors)
  if(strchr(sdp->origin.username,' ') != NULL)
    return -1;
  if(sdp->origin.sess_id < 0)
    return -1;
  if(sdp->origin.sess_version < 0)
    return -1;
  if(sdp->origin.nettype == NULL)
    return -1;
  if(strcmp(sdp->origin.nettype,"IN"))
    return -1;
  if(sdp->origin.addrtype == NULL)
    return -1;
  if(strcmp(sdp->origin.addrtype,"IP4")&&strcmp(sdp->origin.addrtype,"IP6"))
    return -1;
  if(sdp->origin.unicast_address == NULL)
    return -1;

  str[0]='\0';
  /*
    v=0
    o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
    s=<session name>
  */
  sprintf(str,"v=0\no=%s %i %i %s %s %s\ns=%s",
          sdp->origin.username,
          sdp->origin.sess_id,
          sdp->origin.sess_version,
          sdp->origin.nettype,
          sdp->origin.addrtype,
          sdp->origin.unicast_address,
          sdp->session.name
          );

  //return 0;

  /*
    Optional SDP fields
  */

  //i=<session description>
  if(sdp->session.info != NULL){
    strcat(str,"\ni=");
    strcat(str,sdp->session.info);
  }

  //u=<uri>
  if(sdp->URI != NULL){
    strcat(str,"\nu=");
    strcat(str,sdp->URI);
  }

  //c=<nettype> <addrtype> <connection-address>
  int cCount=strcat_connections(sdp->connections,sdp->connectionsCount,str);

  //b=<bwtype>:<bandwidth>
  strcat_bandwidths(sdp->bandwidths,sdp->bandwidthsCount,str);

  char *buff=(char *) malloc(sizeof(char)*1024);

  //t=<start-time> <stop-time>
  if(sdp->start_time < sdp->stop_time && sdp->start_time >= 0){
    sprintf(buff,"\nt=%lli %lli",
            sdp->start_time,
            sdp->stop_time);
    strcat(str,buff);
  }
  buff[0]=0;

  //k=<method> OR k=<method>:<encryption key>
  if(sdp->encryption.method != NULL){
    if(strlen(sdp->encryption.key)>0)
      sprintf(buff,"\nk=%s:%s",
              sdp->encryption.method,
              sdp->encryption.key);
    else
      sprintf(buff,"\nk=%s",
              sdp->encryption.method);
  }

  //a=<attribute> OR a=<attribute>:<value>
  strcat_attributes(sdp->attributes,sdp->attributesCount,str);


  //Media section
  if(sdp->mediums != NULL && sdp->mediumsCount > 0)
    for(int i;i<sdp->mediumsCount;i++){
      //m=<media> <port> <proto> <fmt>
      if(sdp->mediums[i].type != NULL && strlen(sdp->mediums[i].type) > 0 &&
         sdp->mediums[i].port != NULL && strlen(sdp->mediums[i].port) > 0 &&
         sdp->mediums[i].proto != NULL && strlen(sdp->mediums[i].proto) > 0 &&
         sdp->mediums[i].format != NULL && strlen(sdp->mediums[i].format) > 0){
        sprintf(buff,"\nm=%s %s %s %s",
                sdp->mediums[i].type,
                sdp->mediums[i].port,
                sdp->mediums[i].proto,
                sdp->mediums[i].format);
        strcat(str,buff);
        buff[0]='\0';
        //optional media fields
        //i=<media title>
        if(sdp->mediums[i].title != NULL && strlen(sdp->mediums[i].title) > 0){
          sprintf(buff,"i=%s",sdp->mediums[i].title);
          strcat(str,buff);
          buff[0]='\0';
        }

        //c=<nettype> <addrtype> <connection-address>
        if(strcat_connections(sdp->mediums[i].connections,sdp->mediums[i].connectionsCount,str)<=0&&!cCount)
          return -1;

        //b=<bwtype>:<bandwidth>
        if(strcat_bandwidths(sdp->mediums[i].bandwidths,sdp->mediums[i].bandwidthsCount,str)<0)
          return -1;

        //k=<method> OR k=<method>:<encryption key>
        if(sdp->encryption.method != NULL){
          if(strlen(sdp->encryption.key)>0)
            sprintf(buff,"\nk=%s:%s",
                    sdp->encryption.method,
                    sdp->encryption.key);
          else
            sprintf(buff,"\nk=%s",
                    sdp->encryption.method);
        }

        //a=<attribute> OR a=<attribute>:<value>
        strcat_attributes(sdp->mediums[i].attributes,sdp->mediums[i].attributesCount,str);
      }
    }
  strcat(str,buff);
}



int strcat_connections(struct SdpConnectionData *connections,int connectionsCount,char *str){
  int ret=0;
  if(connectionsCount > 0 && connections != NULL)
    for(int i=0;i<connectionsCount;i++){
      if(connections[i].nettype == NULL  ||
         connections[i].addrtype == NULL ||
         connections[i].address == NULL  )
        continue;
      else if(//strchr(connections[i].nettype,' ') != NULL || //only one value - IN
              strcmp(connections[i].nettype,"IN") ||
              //strchr(sdp->connections[i].addrtype,' ') != NULL || //only two value - IP4/IP6
              (strcmp(connections[i].addrtype,"IP4") &&
              strcmp(connections[i].addrtype,"IP6")) ||
              strchr(connections[i].address,' ') != NULL)
        continue;
      else
      {
        char *p = (char *) malloc(sizeof(char)*(14+strlen(connections[i].address)));
        sprintf(p,"\nc=%s %s %s",
                connections[i].nettype,
                connections[i].addrtype,
                connections[i].address);
        strcat(str,p);
        free(p);
        ret++;
      }
    }
  return ret;
}



int strcat_bandwidths(struct SdpBandwidth *bandwidths,int bandwidthsCount,char *str){
  int ret=0;
  if(bandwidthsCount > 0 && bandwidths != NULL)
    for(int i=0;i<bandwidthsCount;i++){
      if(bandwidths[i].bwtype == NULL ||
         bandwidths[i].bandwidth == NULL)
        continue;
      else if(//strchr(bandwidths[i].bwtype,' ') || //only two values CT/AS
              (strcmp(bandwidths[i].bwtype,"CT") &&
              strcmp(bandwidths[i].bwtype,"AS")) ||
              strchr(bandwidths[i].bandwidth,' ') != NULL)
        continue;
      else{
        char *p=(char *) malloc(sizeof(char)*(7+strlen(bandwidths[i].bandwidth)));
        sprintf(p,"\nb=%s:%s",
                bandwidths[i].bwtype,
                bandwidths[i].bandwidth);
        strcat(str,p);
        free(p);
        ret++;
      }
    }
  return ret;
}



int strcat_attributes(struct SdpAttribute *attributes,int attributesCount,char *str){
  char *buff=(char *) malloc(sizeof(char)*1024);
  buff[0]='\0';
  if(attributes != NULL && attributesCount > 0)
    for(int i=0;i<attributesCount;i++)
      if(attributes[i].attribute != NULL){
        if(attributes[i].value != NULL){
          if(strlen(attributes[i].value)>0)
            sprintf(buff,"\nk=%s:%s",
                    attributes[i].attribute,
                    attributes[i].value);
        }else
          sprintf(buff,"\nk=%s",
                  attributes[i].attribute);
        strcat(str,buff);
      }else
        continue;
}


/////////////////////////////
////////////////////////////
//SDP MANAGE FUNCTIONS/////
//////////////////////////
/////////////////////////


int SdpContent_init(struct SdpContent *sdp){
  sdp->version=0;

  sdp->origin.username=NULL;
  sdp->origin.sess_id=0;
  sdp->origin.sess_version=0;
  sdp->origin.nettype=NULL;
  sdp->origin.addrtype=NULL;
  sdp->origin.unicast_address=NULL;

  sdp->session.name=NULL;
  sdp->session.info=NULL;

  sdp->URI=NULL;
  sdp->connections=NULL;
  sdp->connectionsCount=0;

  sdp->bandwidths=NULL;
  sdp->bandwidthsCount=0;

  sdp->start_time=0;
  sdp->stop_time=0;

  sdp->encryption.method=NULL;
  sdp->encryption.key=NULL;

  sdp->attributes=NULL;
  sdp->attributesCount=0;

  sdp->mediums=NULL;
  sdp->mediumsCount=0;
  return 0;
}

int SdpContent_setVersion(struct SdpContent *sdp,int version){
  if(sdp != NULL){sdp->version=version;return 0;}
  return -1;
}

int SdpContent_setOrigin(struct SdpContent *sdp,char *username,int sess_id,int sess_version,char *nettype,char *addrtype,char *unicast_address){
  if(sdp != NULL && username != NULL && nettype != NULL &&
     addrtype != NULL && unicast_address != NULL){
    if(strchr(username,' ') != NULL || sess_id<0 || sess_version<0 ||
        strcmp(nettype,"IN") || (strcmp(addrtype,"IP4") &&
        strcmp(addrtype,"IP6")) || strchr(addrtype,' ') != NULL)
      return -1;

    if(sdp->origin.username != NULL)
      free(sdp->origin.username);
    sdp->origin.username=malloc(strlen(username));
    if(sdp->origin.username == NULL)
      return -1;
    strcpy(sdp->origin.username,username);

    sdp->origin.sess_id=sess_id;
    sdp->origin.sess_version=sess_version;

    sdp->origin.nettype="IN";

    if(sdp->origin.addrtype != NULL)
      free(sdp->origin.addrtype);
    sdp->origin.addrtype=malloc(strlen(addrtype));
    if(sdp->origin.addrtype == NULL)
      return -1;
    strcpy(sdp->origin.addrtype,addrtype);

    if(strchr(unicast_address,' ') != NULL)
      return -1;
    sdp->origin.unicast_address=(char *) malloc(strlen(unicast_address));
    if(sdp->origin.unicast_address == NULL)
      return -1;
    strcpy(sdp->origin.unicast_address,unicast_address);
    return 0;
  }
  return -1;
}

int SdpContent_setSession(struct SdpContent *sdp,char *name,char *info){
  if(sdp != NULL && name != NULL){
    if(sdp->session.name != NULL)
      free(sdp->session.name);
    sdp->session.name=malloc(strlen(name));
    if(sdp->session.name == NULL)
      return -1;
    strcpy(sdp->session.name,name);
    if(info != NULL){
      if(sdp->session.info != NULL)
        free(sdp->session.info);
      sdp->session.info=malloc(strlen(info));
      if(sdp->session.info == NULL)
        return -1;
      strcpy(sdp->session.info,info);
    }
    return 0;
  }
  return -1;
}

int SdpContent_setUri(struct SdpContent *sdp,char *URI){
  if(sdp != NULL && URI != NULL && strchr(URI,' ') == NULL){
    if(sdp->URI != NULL)
      free(sdp->URI);
    sdp->URI=malloc(strlen(URI));
    if(sdp->URI == NULL)
      return -1;
    strcpy(sdp->URI,URI);
    return 0;
  }
  return -1;
}

int SdpContent_addConnection(struct SdpContent *sdp,char *nettype,char *addrtype,char *address){
  if(sdp != NULL && nettype != NULL && addrtype != NULL && address != NULL){
    if(sdp->connectionsCount <= 0 || sdp->connections == NULL){
      sdp->connections=(struct SdpConnectionData *) malloc(sizeof(struct SdpConnectionData));
      sdp->connectionsCount=0;
    } else {
      sdp->connections=(struct SdpConnectionData *) realloc((void *)sdp->connections,sizeof(struct SdpConnectionData)*(sdp->connectionsCount+1));
    };
    if(sdp->connections == NULL)
      return -1;
    
    sdp->connections[sdp->connectionsCount].nettype=malloc(strlen(nettype));
    if(sdp->connections[sdp->connectionsCount].nettype == NULL)
      return -1;
    strcpy(sdp->connections[sdp->connectionsCount].nettype,nettype);

    sdp->connections[sdp->connectionsCount].addrtype=malloc(strlen(addrtype));
    if(sdp->connections[sdp->connectionsCount].addrtype == NULL)
      return -1;
    strcpy(sdp->connections[sdp->connectionsCount].addrtype,addrtype);
    
    sdp->connections[sdp->connectionsCount].address=malloc(strlen(address));
    if(sdp->connections[sdp->connectionsCount].address==NULL)
      return -1;
    strcpy(sdp->connections[sdp->connectionsCount].address,address);


    return sdp->connectionsCount+=1;
  }
  return -1;
}

int SdpContent_addBandwidth(struct SdpContent *sdp,char *bwtype,char *bandwidth){
  if(sdp != NULL && bwtype != NULL ){
    if(sdp->bandwidthsCount <= 0 || sdp->bandwidths == NULL){
      sdp->bandwidths=(struct SdpBandwidth *) malloc(sizeof(struct SdpBandwidth));
      sdp->bandwidthsCount=0;
    }else{
      sdp->bandwidths=(struct SdpBandwidth *) realloc((void *)sdp->bandwidths,sizeof(struct SdpBandwidth)*(sdp->bandwidthsCount+1));
    }
    if(sdp->bandwidths == NULL)
      return -1;

    sdp->bandwidths[sdp->bandwidthsCount].bwtype=(char*) malloc((strlen(bwtype)+1));
    strcpy(sdp->bandwidths[sdp->bandwidthsCount].bwtype,bwtype);
    if(bandwidth == NULL)
      sdp->bandwidths[sdp->bandwidthsCount].bandwidth=NULL;
    else {
      sdp->bandwidths[sdp->bandwidthsCount].bandwidth=(char *) malloc((strlen(bandwidth)+1));
      strcpy(sdp->bandwidths[sdp->bandwidthsCount].bandwidth,bandwidth);
    }

    return sdp->bandwidthsCount+=1;
  }
  return -1;
}

int SdpContent_setEncryption(struct SdpContent *sdp,char *method,char *key){
  if(sdp != NULL && method != NULL){
    if(sdp->encryption.key != NULL)
      free(sdp->encryption.key);
    sdp->encryption.method=malloc(strlen(method));
    strcpy(sdp->encryption.method,method);
    if(key!=NULL){
      sdp->encryption.key=malloc(strlen(key));
      strcpy(sdp->encryption.key,key);
    } else {
      sdp->encryption.key = NULL;
    }
  }
}