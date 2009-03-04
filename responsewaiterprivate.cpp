#include "responsewaiterprivate.h"
#include "responsewaiter.h"
#include "responsewaiterprivate.moc"
namespace protorpc{
ResponseWaiterPrivate::ResponseWaiterPrivate(ResponseWaiter *parent)
{
    this->parent=parent;
}
void ResponseWaiterPrivate::channelBroken(RpcChannel b) {
        parent->wl.lock();
        cbr = null;
        responded = true;
        parent->wc.wakeAll();
        parent->wl.unlock();
}


void ResponseWaiterPrivate::callback(E param) {
        parent->wl.lock();
        parent->cbr = param;
        parent->responded = true;
        parent->wc.wakeAll();
        parent->wl.unlock();
}


void ResponseWaiterPrivate::methodCanceled() {
        wl.lock();
        try {
                cbr = null;
                responded = true;
                wc.signalAll();
        } finally {
                wl.unlock();
        }
}
void ResponseWaiterPrivate::methodFailed(String reason) {
        wl.lock();
        try {
                cbr = null;
                responded = true;
                wc.signalAll();
        } finally {
                wl.unlock();
        }
}

};
