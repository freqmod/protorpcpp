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

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QThread>
#include <QTcpServer>
#include <QSet>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "twowaystream.h"
namespace protorpc{
class SocketServer : public QThread
{
public:
    SocketServer(google::protobuf::Service* toServe,QHostAddress addr,uint16_t port);
    ~SocketServer();
    QTcpServer *getTcpServer();
    //static void shutdownCallback(SocketServer *on,TwoWayStream **tws);

    void start();
    void shutdown();
    bool doShutDownOnDisconnect();
    void setShutDownOnDisconnect(bool shutDownOnDisconnect) ;
    void run();
    //void shutdownCallback(SocketServer *on);

protected:
    QTcpServer* tsr;
    QMutex *soclock;
    //QWaitCondition socwait;
    QHostAddress addr;
    uint16_t prt;
    bool running;
    bool prepared;
    bool shutdownOnDisconnect;
    google::protobuf::Service* service;
    QSet<TwoWayStream**> streamServers;
    void stopServer(TwoWayStream** tws);
};
};
#endif // SOCKETSERVER_H
