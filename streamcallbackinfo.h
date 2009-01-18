#ifndef STREAMCALLBACKINFO_H
#define STREAMCALLBACKINFO_H
namespace protorpc{

class TwoWayStream;
class RpcController;
class StreamCallbackInfo
{
public:
    StreamCallbackInfo();
    StreamCallbackInfo(TwoWayStream *str,RpcController *ctr,uint32_t id);
    TwoWayStream *str;
    RpcController *ctr;
    uint32_t id;
};
}
#endif // STREAMCALLBACKINFO_H
