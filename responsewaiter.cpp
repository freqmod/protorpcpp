#include "responsewaiterprivate.h"
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

/**
 * <p>
 * This class implements a way to make non blocking Rpc calls blocking. It may
 * be used like this:
 * </p>
 *
 * <pre>
 * ResponseWaiter&lt;E&gt; waiter = new ResponseWaiter&lt;E&gt;(breakableRpcChannel);
 * SimpleRpcController cont=new SimpleRpcController();
 * service.callMethod(cont, request, waiter.getCallback());
 * try {
 * 	E response = waiter.await();
 * 	waiter.cleanup();
 *
 * 	//handle response
 *
 * } catch (InterruptedException e) {
 * 	//handle exception
 * } catch (TimeoutException e) {
 * 	//handle exception
 * }
 * waiter.reset(rpcChannel);//if you want to use it again
 * </pre>
 *
 * @author Frederik
 *
 * @param <E>
 *            - the callback type for the Rpc call
 */



namespace protorpc{



/**
 * @param ch Channel that should notify the waiter if it is broken
 * @param co Controller that should notify the waiter if the call
 * 			is canceled or fails
 */
ResponseWaiter::ResponseWaiter(QObject *ch, QObject *ctrl,google::protobuf::Message *resp) {
    child=new ResponseWaiterPrivate(this,resp);
    al= new QMutex(QMutex::Recursive);
    wl= new QMutex();
    listen(ch, ctrl,resp);
}
ResponseWaiter::~ResponseWaiter(){
    delete child;
    delete al;
    delete wl;
}

/**
 * Wait for the method to return
 *
 * @param timeout
 *            - the time in TimeUnit before returning timeout exception
 * @param unit
 *            - the time unit the timeout is specified in
 * @return Response
 * @throws InterruptedException
 *             if the thread is interrupted
 * @throws TimeoutException
 *             if the waiting timed out
 */
google::protobuf::Message *ResponseWaiter::wait(long timeout){
        QMutexLocker wlckr(wl);
        QMutexLocker alckr(al);
        if (responded)
                return cbr;
        if (timeout == 0)
            wc.wait(wl);
        else{
            if(!wc.wait(wl,timeout)){//timed out
                return NULL;
            }
        }

        if (responded)
                return cbr;
        else
                return NULL;

}

/**
 * Reset the waiter to make it wait for new responses
 *
 * @param newchan
 *            if null, or doesnt implement BreakableRpcChannel the waiter
 *            will wait infinitly if the channel is broken and it doesn't
 *            timeout
 */
void ResponseWaiter::reset(QObject *newchan,QObject *newco,google::protobuf::Message* resp) {
        if (al->tryLock()) {
            cbr = NULL;
            responded = false;
            cleanup();
            listen(newchan,newco,resp);
            al->unlock();
        } else {
            throw "The response is allready waiting on something";
        }
}

/**
 * Clean up the waiter after use and remove the pointer to the channel
 */
void ResponseWaiter::cleanup() {
    if (al->tryLock()) {
        child->cleanup();
        al->unlock();
    } else {
        throw "The response is allready waiting on something";
    }
}

void ResponseWaiter::listen(QObject *chan, QObject *ctrl,google::protobuf::Message* resp) {
    if (al->tryLock()) {
        child->connectSignals(chan,ctrl);
        child->changeResponse(resp);
        cbr=resp;
        responded=false;
        al->unlock();
    } else {
        throw "The response is allready waiting on something";
    }
}

google::protobuf::Closure *ResponseWaiter::getClosure(){
    return child->closure;
}


void ResponseWaiter::channelBroken(google::protobuf::RpcChannel *b) {
        QMutexLocker wlcker(wl);
        cbr = NULL;
        responded = true;
        printf("RW:Broken\n");
        wc.wakeAll();
}

void ResponseWaiter::callback(google::protobuf::Message *param) {
        QMutexLocker wlcker(wl);
        cbr=param;
        responded = true;
        printf("RW:Callback\n");
        wc.wakeAll();
}
void ResponseWaiter::methodCanceled(google::protobuf::RpcController *ctrl){
        QMutexLocker wlcker(wl);
        cbr = NULL;
        responded = true;
        printf("RW:Canceled\n");
        wc.wakeAll();
}
void ResponseWaiter::methodFailed(google::protobuf::RpcController *ctrl,std::string reason){
        QMutexLocker wlcker(wl);
        cbr = NULL;
        responded = true;
        printf("RW:Failed\n");
        wc.wakeAll();
}

};
