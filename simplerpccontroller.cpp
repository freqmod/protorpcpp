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
