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
      Or if not 0, error.
      All errors codes defined at enum 
      https://tools.ietf.org/html/rfc4566#section-5.1
    */
    int version;

    /*
      Origin ("o=").
      o=<username> <sess-id> <sess-version> <nettype> <addrtype>
        <unicast-address>
      https://tools.ietf.org/html/rfc4566#section-5.2
    */
    union{
      char *username;
      int sess_id;
      int sess_version;
      char *nettype;
      char *addrtype;
      char *unicast_address;
    }origin;

    union{
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

    int bandwithsCount;

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
    union SdpEncryption{
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
      union SdpEncryption encryption;
      struct SdpConnectionData *connections;
      int connectionsCount;
    } *mediums;

    int mediumsCount;
  };

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


#endif 