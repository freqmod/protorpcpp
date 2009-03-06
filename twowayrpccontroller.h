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

#ifndef TWOWAYRPCCONTROLLER_H
#define TWOWAYRPCCONTROLLER_H
#include "twowaystream.h"
#include "simplerpccontroller.h"
#include <google/protobuf/service.h>
namespace protorpc{
class TwoWayRpcController : public SimpleRpcController
{
public:
    TwoWayRpcController(TwoWayStream *str);
    void * getSessionId();
    void setSessionId(void *sid);
    TwoWayStream *getTwoWayStream();
    google::protobuf::RpcChannel *getRpcChannel();
protected:
    TwoWayStream* strm;
};
}
#endif // TWOWAYRPCCONTROLLER_H
