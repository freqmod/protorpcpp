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
    QMutex soclock;
    QWaitCondition socwait;
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
