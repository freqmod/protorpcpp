#ifndef TWOWAYRPCCONTROLLER_H
#define TWOWAYRPCCONTROLLER_H
#include "twowaystream.h"
#include "simplerpccontroller.h"
#include <google/protobuf/service.h>
namespace protorpc{
class TwoWayRpcController : public SimpleRpcController
{
public:
    TwoWayRpcController(TwoWayStream *str);
    void * getSessionId();
    void setSessionId(void *sid);
    TwoWayStream *getTwoWayStream();
    google::protobuf::RpcChannel *getRpcChannel();
protected:
    TwoWayStream* strm;
};
}
#endif // TWOWAYRPCCONTROLLER_H
