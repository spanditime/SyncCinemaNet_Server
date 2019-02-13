#ifndef SDP
  #define SDP

  /*
    Look at SDP specification
    https://tools.ietf.org/html/rfc4566#page-7
  */
  typedef struct SDP_{
    int version;
    union{
      char *username;
      char *sess_id;
      int sess_version;
      char *nettype;
      char *addrtype;
      char *unicast_address;
    }origin;
    union{
      char *name;
      char *info;
    }session;
    char *URI;
  } SdpContent;

  int parse_sdp(SdpContent *sdp,char *str);

  int sdpstr(char *str,SdpContent *sdp);


#endif 