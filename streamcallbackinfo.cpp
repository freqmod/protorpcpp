#include "streamcallbackinfo.h"
#include "twowaystream.h"
#include <stdint.h>
namespace protorpc{

StreamCallbackInfo::StreamCallbackInfo()
{
}
StreamCallbackInfo::StreamCallbackInfo(TwoWayStream *str,RpcController *ctr,google::protobuf::Message *msg,uint32 id){
    this->str=str;
    this->ctr=ctr;
    this->id=id;
    this->msg=msg;
}
static void StreamCallbackInfo::callhack(StreamCallbackInfo *scbi){
    scbi->str->response(this);
}
