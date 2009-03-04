#include "responsewaiter.h"

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







/**
 * @param ch Channel that should notify the waiter if it is broken
 * @param co Controller that should notify the waiter if the call
 * 			is canceled or fails
 */
ResponseWaiter::ResponseWaiter(QObject *ch, CallbackRpcController *ctrl) {
        listen(ch, co);
}

/**
 * Wait for the method to return
 *
 * @return Response
 * @throws InterruptedException
 *             if the thread is interrupted
 */
E ResponseWaiter::await(){
    return await(0, null);
}

/**
 * Wait for the method to return
 *
 * @param timeout
 *            - the time in millisecounds before returning timeout exception
 * @return Response
 * @throws InterruptedException
 *             if the thread is interrupted
 * @throws TimeoutException
 *             if the waiting timed out
 */
E ResponseWaiter::await(long timeout){
        return await(timeout, TimeUnit.MILLISECONDS);
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
E ResponseWaiter::await(long timeout, TimeUnit unit){
        if (responded)
                return cbr;
        wl.lock();
        al.lock();
        try {
                if (timeout == 0)
                        wc.await();
                else
                        wc.await(timeout, unit);
                if (responded)
                        return cbr;
                else
                        throw new TimeoutException("Callback timed out");
        } finally {
                wl.unlock();
                al.unlock();
        }

}

/**
 * Reset the waiter to make it wait for new responses
 *
 * @param newchan
 *            if null, or doesnt implement BreakableRpcChannel the waiter
 *            will wait infinitly if the channel is broken and it doesn't
 *            timeout
 */
void ResponseWaiter::reset(QObject *newchan,CallbackRpcController *newco) {
        if (al.tryLock()) {
                try {
                        cbr = null;
                        responded = false;
                        cleanup();
                        listen(newchan,newco);
                } finally {
                        al.unlock();
                }
        } else {
                throw new RejectedExecutionException(
                                "The response is allready waiting on something");
        }
}

/**
 * Clean up the waiter after use and remove the pointer to the channel
 */
void ResponseWaiter::cleanup() {
        if (bc != NULL) {
                bc.removeChannelBrokenListener(priv);
        }
        if (co != NULL) {
                bc.removeChannelBrokenListener(priv);
        }

}

void ResponseWaiter::listen(QObject *ch, CallbackRpcController *ctrl) {
        if (bc != null ) {
                this.bc=bc;
                bc.addChannelBrokenListener(priv);
        }
        if (co != null) {
                this.co=co;
                co.addControllerInfoListener(priv);
        }
}

RpcCallback<E> ResponseWaiter::getCallback() {
        return priv;
}

class ResponseWaiterPrivate : public QObject{
}
class ResponseWaiterPrivate{// implements RpcCallback<E>,
                //ChannelBrokenListener,ControllerInfoListener {
        public void channelBroken(RpcChannel b) {
                wl.lock();
                try {
                        cbr = null;
                        responded = true;
                        wc.signalAll();
                } finally {
                        wl.unlock();
                }
        }

        @Override
        public void run(E param) {
                wl.lock();
                try {
                        cbr = param;
                        responded = true;
                        wc.signalAll();
                } finally {
                        wl.unlock();
                }
        }

        @Override
        public void methodCanceled() {
                wl.lock();
                try {
                        cbr = null;
                        responded = true;
                        wc.signalAll();
                } finally {
                        wl.unlock();
                }
        }

        @Override
        public void methodFailed(String reason) {
                wl.lock();
                try {
                        cbr = null;
                        responded = true;
                        wc.signalAll();
                } finally {
                        wl.unlock();
                }
        }
}
}
