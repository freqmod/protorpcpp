#ifndef RESPONSEWAITER_H
#define RESPONSEWAITER_H
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
namespace protorpc{
class ResponseWaiterPrviate;
template<class E>
class ResponseWaiter
{
public:
    ResponseWaiter();
private:
        bool responded;
        // RpcCallback<E> cb;
        QMutex wl;// = new ReentrantLock();
        QWaitCondition wc;// = wl.newCondition();
        QMutex al;//ReentrantLock al = new ReentrantLock();
        E *cbr;
        QObject *bc;
        //private ResponseWaiterPrivate priv = new ResponseWaiterPrivate();
        ResponseWaiterPrivate *child;
        QObject *ctrl;//rpc controller with signals
};
};
#endif // RESPONSEWAITER_H
