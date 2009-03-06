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

#ifndef SIMPLERPCCONTROLLER_H
#define SIMPLERPCCONTROLLER_H

#include <QObject>
#include <QSet>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

namespace protorpc{
class SimpleRpcController : public QObject, public google::protobuf::RpcController
{
    Q_OBJECT;
public:
    SimpleRpcController();
    //client side
    std::string ErrorText() const;
    bool Failed() const;
    void StartCancel();
    void Reset();
    //server side
    void SetFailed(const std::string &reason);
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure *cb);
    void startCancel();
    //extensions
signals:
    void methodFailed(google::protobuf::RpcController *ctrl,std::string reason);
    void methodCanceled(google::protobuf::RpcController *ctrl);
protected:
    std::string reason;
    bool hasFailed;
    bool canceled;
    QSet<google::protobuf::Closure*> cancelListeners;
};
}
//#include "simplerpccontroller.moc"
#endif // SIMPLERPCCONTROLLER_H

