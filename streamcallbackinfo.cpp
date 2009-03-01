#include "streamcallbackinfo.h"
#include "twowaystream.h"
#include <stdint.h>

namespace protorpc{

StreamCallbackInfo::StreamCallbackInfo()
{
}
StreamCallbackInfo::StreamCallbackInfo(TwoWayStream *str,google::protobuf::RpcController *ctr,google::protobuf::Message *msg,uint32_t id){
    this->str=str;
    this->ctr=ctr;
    this->id=id;
    this->msg=msg;
}
void StreamCallbackInfo::callhack(StreamCallbackInfo *scbi){
    scbi->str->response(scbi);
}
}
