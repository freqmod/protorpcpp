#ifndef TWOWAYRPCCONTROLLER_H
#define TWOWAYRPCCONTROLLER_H
#include "twowaystream.h"
namespace protorpc{
class TwoWayRpcController : public SimpleRpcController
{
public:
    TwoWayRpcController();
    void * getSessionId();
    void setSessionId(void *sid);
    TwoWayStream *getTwoWayStream();
    RpcChannel *getRpcChannel();
protected:
    TwoWayStream* strm;
};
}
#endif // TWOWAYRPCCONTROLLER_H
