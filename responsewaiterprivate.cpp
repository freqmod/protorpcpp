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

#include "responsewaiterprivate.h"
#include "responsewaiter.h"
#include "responsewaiterprivate.moc"
namespace protorpc{
ResponseWaiterPrivate::ResponseWaiterPrivate(ResponseWaiter *parent,google::protobuf::Message *resp)
{
    this->parent=parent;
    closure=google::protobuf::NewPermanentCallback(staticCallback,this,resp);
}
ResponseWaiterPrivate::~ResponseWaiterPrivate(){
    delete closure;
}
void ResponseWaiterPrivate::changeResponse(google::protobuf::Message *resp) {
    QMutexLocker(parent->al);
    if(closure)
        delete closure;
    closure=google::protobuf::NewPermanentCallback(staticCallback,this,resp);
}


void ResponseWaiterPrivate::channelBroken(google::protobuf::RpcChannel *b) {
    parent->channelBroken(b);
}
void ResponseWaiterPrivate::callback(google::protobuf::Message* param){
    parent->callback(param);
}
void ResponseWaiterPrivate::staticCallback(ResponseWaiterPrivate* ths,google::protobuf::Message *param){
    ths->callback(param);
}
void ResponseWaiterPrivate::methodCanceled(google::protobuf::RpcController *ctrl){
    parent->methodCanceled(ctrl);
}
void ResponseWaiterPrivate::methodFailed(google::protobuf::RpcController *ctrl,std::string reason){
    parent->methodFailed(ctrl,reason);
}

google::protobuf::Closure *ResponseWaiterPrivate::getClosure(){
    return closure;
}
void ResponseWaiterPrivate::connectSignals(QObject *chan,QObject *ctrl){
    QMutexLocker(parent->al);
    if(chan!=NULL)
        connect(chan,SIGNAL(channelBroken(google::protobuf::RpcChannel*)),this,SLOT(channelBroken(google::protobuf::RpcChannel*)),Qt::DirectConnection);
    if(ctrl!=NULL){
        connect(ctrl,SIGNAL(methodCanceled(google::protobuf::RpcController*)),this,SLOT(methodCanceled(google::protobuf::RpcController*)),Qt::DirectConnection);
        connect(ctrl,SIGNAL(methodFailed(google::protobuf::RpcController*,std::string)),this,SLOT(methodFailed(google::protobuf::RpcController*,std::string)),Qt::DirectConnection);
    }
}
void ResponseWaiterPrivate::cleanup(){
    QMutexLocker(parent->al);
    printf("Prediscon\n");
    disconnect(this);
    printf("Postdiscon\n");
    /*disconnect(this,SLOT(channelBroken(google::protobuf::RpcChannel*)));
    disconnect(this,SLOT(methodCanceled(google::protobuf::RpcController*)));
    disconnect(this,SLOT(methodFailed(google::protobuf::RpcController*,std::string)));*/
}
};



