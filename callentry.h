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

#ifndef CALLENTRY_H
#define CALLENTRY_H

#include <QObject>
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
namespace protorpc{

class CallEntry : public QObject
{
    //FIXME:Think out memory management
public:
    CallEntry();
    CallEntry(google::protobuf::RpcController *ctrl,google::protobuf::Closure *cb,google::protobuf::Message *resp);
    CallEntry(const CallEntry &ce);
    CallEntry(const CallEntry *ce);
    void operator=(const CallEntry &ce);
    google::protobuf::RpcController *ctrl;
    google::protobuf::Closure *callback;
    google::protobuf::Message *resp;
};
}
#endif // CALLENTRY_H
