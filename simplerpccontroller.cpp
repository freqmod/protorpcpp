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

#include "simplerpccontroller.h"
#include "simplerpccontroller.moc"
namespace protorpc{

SimpleRpcController::SimpleRpcController(){
    Reset();
}
//client side
std::string SimpleRpcController::ErrorText() const{
    return reason;
}
bool SimpleRpcController::Failed() const{
    return hasFailed;
}
void SimpleRpcController::StartCancel(){
    canceled=true;
    foreach(google::protobuf::Closure* cb,cancelListeners)
        cb->Run();
    emit methodCanceled(this);
}
void SimpleRpcController::Reset(){
    reason="";
    hasFailed=false;
    canceled=false;
    cancelListeners.clear();
    disconnect(this,SIGNAL(methodFailed(google::protobuf::RpcController*,std::string)),0,0);//disconnect all info listeners
    disconnect(this,SIGNAL(methodCanceled(google::protobuf::RpcController*)),0,0);//disconnect all info listeners
}
//server side
void SimpleRpcController::SetFailed(const std::string &reason){
    this->hasFailed=true;
    this->reason=reason;
    emit methodFailed(this,reason);
}
bool SimpleRpcController::IsCanceled() const{
    return canceled;
}
void SimpleRpcController::NotifyOnCancel(google::protobuf::Closure *cb){
    cancelListeners.insert(cb);
}
}
