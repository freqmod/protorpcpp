#include "simplerpccontroller.h"
#include "simplerpccontroller.moc"
namespace protorpc{

SimpleRpcController::SimpleRpcController(){
    Reset();
}
//client side
std::string SimpleRpcController::ErrorText(){
    return reason;
}
bool SimpleRpcController::Failed(){
    return hasFailed;
}
void SimpleRpcController::startCancel(){
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
    disconnect(this,SIGNAL(methodFailed(SimpleRpcController*,std::string)),0,0);//disconnect all info listeners
    disconnect(this,SIGNAL(methodCanceled(SimpleRpcController*)),0,0);//disconnect all info listeners
}
//server side
void SimpleRpcController::setFailed(std::string reason){
    this->hasFailed=true;
    this->reason=reason;
    emit hasFailed(this,reason);
}
bool SimpleRpcController::IsCanceled(){
}
void SimpleRpcController::notifyOnCancel(Closure *cb){
}
}
