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

#include "streamcallbackinfo.h"
#include "twowaystream.h"
#include <stdint.h>

namespace protorpc{

StreamCallbackInfo::StreamCallbackInfo()
{
}
StreamCallbackInfo::StreamCallbackInfo(TwoWayStream *str,google::protobuf::RpcController *ctr,google::protobuf::Message *msg,uint32_t id){
    this->str=str;
    this->ctr=ctr;
    this->id=id;
    this->msg=msg;
}
void StreamCallbackInfo::callhack(StreamCallbackInfo *scbi){
    scbi->str->response(scbi);
}
}
