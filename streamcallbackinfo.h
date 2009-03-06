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

#ifndef STREAMCALLBACKINFO_H
#define STREAMCALLBACKINFO_H
#include <stdint.h>

namespace google{
    namespace protobuf{
        class RpcController;
        class Message;
    }
}
namespace protorpc{

class TwoWayStream;

class StreamCallbackInfo
{
public:
    StreamCallbackInfo();
    StreamCallbackInfo(TwoWayStream *str,google::protobuf::RpcController *ctr,google::protobuf::Message *msg,uint32_t id);
    TwoWayStream *str;
    google::protobuf::RpcController *ctr;
    google::protobuf::Message *msg;
    uint32_t id;
    static void callhack(StreamCallbackInfo *scbi);
};
}
#endif // STREAMCALLBACKINFO_H
