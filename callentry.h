#ifndef CALLENTRY_H
#define CALLENTRY_H

#include <QObject>
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
namespace protorpc{

class CallEntry : public QObject
{
    //FIXME:Think out memory management
public:
    CallEntry();
    CallEntry(google::protobuf::RpcController *ctrl,google::protobuf::Closure *cb,google::protobuf::Message *resp);
    CallEntry(CallEntry &ce);
    void operator=(CallEntry &ce);
    google::protobuf::RpcController *ctrl;
    google::protobuf::Closure *callback;
    google::protobuf::Message *resp;
};
}
#endif // CALLENTRY_H
