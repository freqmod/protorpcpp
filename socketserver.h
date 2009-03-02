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
class SocketServer : QThread
{
public:
    SocketServer(google::protobuf::Service* toServe);
    QTcpServer *getTcpServer();
    static void shutdownCallback(SocketServer *on,bool parameter);
    void start();
    void shutdown(bool closeStreams);
    bool doShutDownOnDisconnect();
    void setShutDownOnDisconnect(bool shutDownOnDisconnect) ;
    void run();
    void shutdownCallback(SocketServer *on);

protected:
    QTcpServer tsr;
    bool running;
    bool shutdownOnDisconnect;
    google::protobuf::Service* service;
    QSet<TwoWayStream*> streamServers;
};
};
#endif // SOCKETSERVER_H
