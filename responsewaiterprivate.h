#ifndef RESPONSEWAITERPRIVATE_H
#define RESPONSEWAITERPRIVATE_H

#include <QObject>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "responsewaiter.h"
//#include "responsewaiterprivate.moc"
namespace protorpc{
template<class E>
class ResponseWaiterPrivate : public QObject
{
    friend class ResponseWaiter;
    Q_OBJECT
public:
    ResponseWaiterPrivate();
    void callback(E param);
    void methodCanceled();
    void methodFailed(String reason);
protected:
    ResponseWaiter *parent;
public slots:
    void channelBroken(google::protobuf::RpcChannel *chan);
};
};

#endif // RESPONSEWAITERPRIVATE_H
