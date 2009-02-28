#include "twowayrpccontroller.h"
namespace protorpc {
TwoWayRpcController::TwoWayRpcController(TwoWayStream *str){
    this->strm=str;
}
void *TwoWayRpcController::getSessionId(){
}
void TwoWayRpcController::setSessionId(void *sid){
}
TwoWayStream *TwoWayRpcController::getTwoWayStream(){
    return strm;
}
RpcChannel *TwoWayRpcController::getRpcChannel(){
}
}
