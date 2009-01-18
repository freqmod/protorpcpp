#include "streamcallbackinfo.h"
namespace protorpc{

StreamCallbackInfo::StreamCallbackInfo()
{
}
StreamCallbackInfo::StreamCallbackInfo(TwoWayStream *str,RpcController *ctr,uint32_t id){
    this->str=str;
    this->ctr=ctr;
    this->id=id;
}
}
