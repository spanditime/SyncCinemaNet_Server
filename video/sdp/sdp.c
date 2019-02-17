#include <string.h>
#include <stdio.h>//sprintf

#include "sdp.h"
#include "interfaces/errors.h"

int parse_sdp(struct SdpContent *sdp,char *str){
  die_err(SE_DEV);
  return -1;
  /*
    parser
  */
}

/**
 * /brief
 * 
 * 
 * /param[out] str Output SDP string
 * /param[in] sdp Input SDP structure for reparse into string str
 * /return Error code or 0 if its done succeful
*/
int sdpstr(char *str,struct SdpContent *sdp){
  die_err(SE_DEV);
  return -1;
  /*
    reparser
    TODO:
    []update return errors codes
  */
  if(sdp == NULL || str == NULL)
    return -1;
  str[0]='\0';

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
  if(!strcmp(sdp->origin.nettype,"IN"))
    return -1;
  if(sdp->origin.addrtype == NULL)
    return -1;
  if(!strcmp(sdp->origin.addrtype,"IP4")||!strcmp(sdp->origin.addrtype,"IP6"))
    return -1;
  if(sdp->origin.unicast_address == NULL)
    return -1;

  /*
    v=0
    o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
    s=<session name>
  */
  sprintf("v=0\no=%s %d %d %s %s %s\ns=%s",
          sdp->origin.username,
          sdp->origin.sess_id,
          sdp->origin.sess_version,
          sdp->origin.nettype,
          sdp->origin.addrtype,
          sdp->origin.unicast_address,
          sdp->session.name
          );
  
  return 0;
}