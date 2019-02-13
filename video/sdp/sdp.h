#ifndef SDP
  #define SDP

  /*
    Look at SDP specification
    https://tools.ietf.org/html/rfc4566#page-7
  */
  typedef struct {
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
    struct SdpConnectionInfo{

    } *connection_info;
    struct SdpBandwidth{
      
    } *bandwidths;
    int bandwithsCount;
    long long int start_time,stop_time;
    union SdpEncryption{
      char *method;
      char *key;
    }encryption;
    struct SdpAttribute{
      char *attribute;
      char *value;
    } attributes;
    struct SdpMedia{
      char *type;
      char *port;
      char *proto;
      char *format;
      char *title;
      char *connection_info;
      struct SdpBandwidth *bandwidths;
      int bandwidthsCount;
      union SdpEncryption encryption;
      struct SdpConnectionInfo connection_info;
    } *mediums;
    int mediumsCount;
  } SdpContent;

  int parse_sdp(SdpContent *sdp,char *str);

  int sdpstr(char *str,SdpContent *sdp);


#endif 