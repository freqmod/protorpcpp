#include "callentry.h"
namespace protorpc{

CallEntry::CallEntry(){}
CallEntry::CallEntry(google::protobuf::RpcController *ctrl,google::protobuf::Closure *cb,google::protobuf::Message *resp){
    this->ctrl=ctrl;
    this>-callback=cb;
    this->resp=resp;
}
CallEntry::CallEntry(CallEntry &ce){
    operator=(ce);
}
void CallEntry::operator=(CallEntry &ce){
    this->ctrl=ce.ctrl;
    this->cb=ce.cb;
    this->resp=ce.resp;
}
}
