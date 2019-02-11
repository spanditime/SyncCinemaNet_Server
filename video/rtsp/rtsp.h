#ifndef RTSP
  #define RTSP

  int rtsp_server(int fd);

  enum RtspMethods{
    RTSP_METHOD_ERROR=-1,
    RTSP_METHOD_OPTIONS=0,
    RTSP_METHOD_DESCRIBE=1,
    RTSP_METHOD_PLAY=2,
    RTSP_METHOD_PAUSE=3,
    RTSP_METHOD_RECORD=4,
    RTSP_METHOD_REDIRECT=5,
    RTSP_METHOD_SETUP=6,
    RTSP_METHOD_ANNOUNCE=7,
    /*not supported yet
    RTSP_METHOD_GET_PARAMETER=8,
    RTSP_METHOD_SET_PARAMETER=9,
    */
    RTSP_METHOD_TEARDOWN=10
  };

  typedef struct RtspMessage_{
    enum RtspMethods method;
    char *content;
    int CSeq;
  } RtspMessage;

#endif
