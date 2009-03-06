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
