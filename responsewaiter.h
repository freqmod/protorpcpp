/* Copyright (C) 2009 Frederik M.J.V

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * LGPL is available on the internet at
 * http://www.gnu.org/licenses/lgpl-2.1.html and from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
*/

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
