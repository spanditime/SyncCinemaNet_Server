#include "sdp/sdp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){
    struct SdpContent sdp;
    
    SdpContent_init(&sdp);

    sdp.version=0;
    
    char *buff=(char *) malloc(sizeof(char)*1024);
    
    int res=sdpstr(buff,&sdp);
    printf("SDP : %d\n%s",res,buff);
}