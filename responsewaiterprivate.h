#ifndef RESPONSEWAITERPRIVATE_H
#define RESPONSEWAITERPRIVATE_H

#include <QObject>
#include <QList>

#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "responsewaiter.h"
//#include "responsewaiterprivate.moc"
namespace protorpc{
class ResponseWaiterPrivate : public QObject
{
    friend class ResponseWaiter;
    Q_OBJECT
public:
    ResponseWaiterPrivate(ResponseWaiter *parent,google::protobuf::Message *resp);
    ~ResponseWaiterPrivate();
    void callback(google::protobuf::Message *param);
    void methodCanceled();
    void methodFailed(std::string reason);
    void cleanup();
    void connectSignals(QObject *chan,QObject *ctrl);
    void changeResponse(google::protobuf::Message *resp);
    google::protobuf::Closure *getClosure();
    static void staticCallback(ResponseWaiterPrivate* ths,google::protobuf::Message *param);
protected:
    ResponseWaiter *parent;
    google::protobuf::Closure* closure;
public slots:
    void channelBroken(google::protobuf::RpcChannel *chan);
    void methodFailed(google::protobuf::RpcController *ctrl,std::string reason);
    void methodCanceled(google::protobuf::RpcController *ctrl);
};
};

#endif // RESPONSEWAITERPRIVATE_H
