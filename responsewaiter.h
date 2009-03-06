#ifndef RESPONSEWAITER_H
#define RESPONSEWAITER_H
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <google/protobuf/service.h>
namespace protorpc{
class ResponseWaiterPrivate;
class ResponseWaiter
{
    friend class ResponseWaiterPrivate;
public:
    ResponseWaiter(QObject *ch, QObject *ctrl,google::protobuf::Message *resp);
    ~ResponseWaiter();
    google::protobuf::Message *wait(long timeout=0);
    void reset(QObject *newchan,QObject *newco,google::protobuf::Message *resp);
    void listen(QObject *chan, QObject *ctrl,google::protobuf::Message *resp);
    void cleanup();
    google::protobuf::Closure *getClosure();
private:

        void channelBroken(google::protobuf::RpcChannel *b);
        void callback(google::protobuf::Message *param);
        void methodCanceled(google::protobuf::RpcController *ctrl);
        void methodFailed(google::protobuf::RpcController *ctrl,std::string reason);

        bool responded;
        // RpcCallback<E> cb;
        QMutex *wl;// = new ReentrantLock();
        QWaitCondition wc;// = wl.newCondition();
        QMutex *al;//ReentrantLock al = new ReentrantLock();
        google::protobuf::Message *cbr;
        QObject *bc;
        //private ResponseWaiterPrivate priv = new ResponseWaiterPrivate();
        ResponseWaiterPrivate *child;
        QObject *ctrl;//rpc controller with signals
};
};
#endif // RESPONSEWAITER_H
