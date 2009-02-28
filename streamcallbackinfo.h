#ifndef STREAMCALLBACKINFO_H
#define STREAMCALLBACKINFO_H
#include <stdint.h>

namespace google{
    namespace protobuf{
        class RpcController;
        class Message;
    }
}
namespace protorpc{

class TwoWayStream;

class StreamCallbackInfo
{
public:
    StreamCallbackInfo();
    StreamCallbackInfo(TwoWayStream *str,google::protobuf::RpcController *ctr,google::protobuf::Message *msg,uint32_t id);
    TwoWayStream *str;
    google::protobuf::RpcController *ctr;
    google::protobuf::Message *msg;
    uint32_t id;
    static void callhack(StreamCallbackInfo *scbi);
};
}
#endif // STREAMCALLBACKINFO_H
