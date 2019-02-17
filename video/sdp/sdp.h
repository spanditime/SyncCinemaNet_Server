/**
 * /file 
 * /brief Header file with structures and function
 * declarations for working with SDP
 * 
 * 
 */
#ifndef SDP
  #define SDP

  /*
    Look at SDP specification
    https://tools.ietf.org/html/rfc4566
    P.S. email and phone numbers don't supported yet
    (look at https://tools.ietf.org/html/rfc4566#section-5.6)
  */
  struct SdpContent{
    /*
      Version of SDP(now always 0)
      v=0
      Or if not 0, error.
      All errors codes defined at enum 
      https://tools.ietf.org/html/rfc4566#section-5.1
    */
    int version;

    /*
      Origin ("o=").
      o=<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
      https://tools.ietf.org/html/rfc4566#section-5.2
    */
    struct {
      char *username;
      int sess_id;
      int sess_version;
      char *nettype;
      char *addrtype;
      char *unicast_address;
    }origin;

    struct {
      /*
        Session name ("s=").
        s=<session name>
        https://tools.ietf.org/html/rfc4566#section-5.3
      */
      char *name;

      /*
        Session Information ("i=").
        i=<session description>
        https://tools.ietf.org/html/rfc4566#section-5.4
      */
      char *info;
    }session;

    /*
      URI ("u=").
      u=<uri>
      https://tools.ietf.org/html/rfc4566#section-5.5
    */
    char *URI;

    /*
      EMAIL AND PHONE NUMBERS MUST BE HERE.
      P.S. but in this moment is not supported and simply ignored
    */

    /*
      Connection Data ("c=").
      c=<nettype> <addrtype> <connection-address>
      https://tools.ietf.org/html/rfc4566#section-5.7
    */
    struct SdpConnectionData{
      /*
        <nettype>
        Network type.
        Initially, "IN" is defined to have the meaning "Internet"
      */
      char *nettype;

      /*
        <addrtype>
        Addres type.
        This allows SDP to be used for sessions that are not IP based.
        This memo only defines IP4 and IP6.
      */
      char *addrtype;

      /*
        <connection-addres>
        Connection address.
        Look at specification:
        https://tools.ietf.org/html/rfc4566#section-5.7
      */
      char *address;
    } *connections;

    int connectionsCount;

    /*
      Bandwidth ("b=")
      b=<bwtype>:<bandwidth>
      https://tools.ietf.org/html/rfc4566#section-5.8
    */
    struct SdpBandwidth{
      /*
        <bwtype>
        Defined two values: CT, AS.
      */
      char *bwtype;

      /*
        <bandwidth>
        Interpreted as kilobits per second by default.
      */
      char *bandwidth;
    } *bandwidths;

    int bandwidthsCount;

    /*
      Timing ("t=")
      t=<start-time> <stop-time>
      https://tools.ietf.org/html/rfc4566#section-5.9
    */
    long long int start_time,stop_time;

    /*
      REPEAT TIMES MUST BE HERE
      P.S. but in this moment is not supported and simply ignored
      https://tools.ietf.org/html/rfc4566#section-5.10
    */

    /*
      TIME ZONES MUST BE HERE
      P.S. but in this moment is not supported and simply ignored
    */

    /*
      Encryption Keys ("k=")
      k=<method>
      OR
      k=<method>:<encryption key>
      https://tools.ietf.org/html/rfc4566#section-5.12
    */
    struct SdpEncryption{
      char *method;
      char *key;
    } encryption;

    /*
      Attributes ("a=")
      a=<attribute>
      OR
      a=<attribute>:<value>
      https://tools.ietf.org/html/rfc4566#section-5.13
    */
    struct SdpAttribute{
      char *attribute;
      char *value;
    } *attributes;

    int attributesCount;

    /*
      Media Descriptions ("m=")
      m=<media> <port> <proto> <fmt> ...
      https://tools.ietf.org/html/rfc4566#section-5.14
    */
    struct SdpMedia{
      char *type;
      char *port;
      char *proto;
      char *format;
      char *title;
      struct SdpBandwidth *bandwidths;
      int bandwidthsCount;
      struct SdpEncryption encryption;
      struct SdpConnectionData *connections;
      int connectionsCount;
      struct SdpAttribute *attributes;
      int attributesCount;
    } *mediums;

    int mediumsCount;
  };

  //SDP MANAGE FUNCTIONS(make doxygen module)

  int SdpContent_init(struct SdpContent *sdp);

  int SdpContent_setVersion(struct SdpContent *sdp,int version);

  int SdpContent_setOrigin(struct SdpContent *sdp,
                           char *username,
                           int sess_id,
                           int sess_version,
                           char *nettype,
                           char *addrtype,
                           char *unicast_address);

  int SdpContent_setSession(struct SdpContent *sdp,
                            char *name,
                            char *info);

  int SdpContent_setUri(struct SdpContent *sdp,char *URI);

  int SdpContent_addConnection(struct SdpContent *sdp,
                               char *nettype,
                               char *addrtype,
                               char *address);

  int SdpContent_addBandwidth(struct SdpContent *sdp,
                              char *bwtype,
                              char *bandwidth);

  int SdpContent_setEncryption_wKey(struct SdpContent *sdp,
                                    char *method,
                                    char *key);
  int SdpContent_setEncryption(struct SdpContent *sdp,char *method);

  int SdpContent_addAtribute_wValue(struct SdpContent *sdp,
                                    char *attribute,
                                    char *value);
  int SdpContent_addAtribute(struct SdpContent *sdp,char *attribute);

  int SdpContent_addMedia(struct SdpContent *sdp,
                          char *type,
                          char *port,
                          char *proto,
                          char *format);
  

  int SdpContent_Media_setTitle(struct SdpContent *sdp,
                                int mediaID,
                                char *title);

  int SdpContent_Media_addBandwidth(struct SdpContent *sdp,
                                    int mediaID,
                                    char *bwtype,
                                    char *bandwith);

  int SdpContent_Media_setEncryption_wKey(struct SdpContent *sdp,
                                          int mediaID,
                                          char *method,
                                          char *key);
  int SdpContent_Media_setEncryption(struct SdpContent *sdp,
                                     int mediaID,
                                     char *method);

  int SdpContent_Media_addAtribute_wValue(struct SdpContent *sdp,
                                          int mediaID,
                                          char *attribute,
                                          char *value);
  int SdpContent_Media_addAtribute(struct SdpContent *sdp,
                                   int mediaID,
                                   char *attribute);


  //SDP MANAGE FUNCTIONS END

  //SDP PARSE FUNCTIONS(make doxygen module)

  /*
    Parse SDP string into SdpContent struct
    More info about SDP:
    https://tools.ietf.org/html/rfc4566
  */
  int parse_sdp(struct SdpContent *sdp,char *str);

  /*
    Reparse SdpContent struct into string
  */
  int sdpstr(char *str,struct SdpContent *sdp);

  //SDP PARSE FUNCTIONS END

#endif 