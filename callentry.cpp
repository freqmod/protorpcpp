#include "callentry.h"
namespace protorpc{

CallEntry::CallEntry(){
    this->ctrl=NULL;
    this->callback=NULL;
    this->resp=NULL;
}
CallEntry::CallEntry(google::protobuf::RpcController *ctrl,google::protobuf::Closure *cb,google::protobuf::Message *resp){
    this->ctrl=ctrl;
    this->callback=cb;
    this->resp=resp;
}
CallEntry::CallEntry(const CallEntry &ce){
    operator=(ce);
}
CallEntry::CallEntry(const CallEntry *ce){
    operator=(ce);
}

void CallEntry::operator=(const CallEntry &ce){
    this->ctrl=ce.ctrl;
    this->callback=ce.callback;
    this->resp=ce.resp;
}

}
