#ifndef STREAMCALLBACKINFO_H
#define STREAMCALLBACKINFO_H
namespace google{
    namespace protobuf{
        class RpcController;
    }
}
namespace protorpc{

class TwoWayStream;

class StreamCallbackInfo
{
public:
    StreamCallbackInfo();
    StreamCallbackInfo(TwoWayStream *str,google::protobuf::RpcController *ctr,uint32_t id);
    TwoWayStream *str;
    google::protobuf::RpcController *ctr;
    uint32_t id;
    static void callhack(StreamCallbackInfo *scbi);
};
}
#endif // STREAMCALLBACKINFO_H
