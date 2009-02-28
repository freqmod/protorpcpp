#ifndef TWOWAYSTREAM_H
#define TWOWAYSTREAM_H

#include <Qt/QtCore>
#include <Qt/QtNetwork>
#include <callentry.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
namespace protorpc{
class TwoWayStream : public QThread
{
protected:
    QIODevice *dev;
    QMutex streamlock;
    bool connected;
private:
    int timeout;
    google::protobuf::Service *service;
    uint32_t callnum;
    bool spawnCallers;
    QHash<uint32_t,CallEntry> currentCalls;
public:
    TwoWayStream(QIODevice *dev,google::protobuf::Service* srv,bool autostart);
    void callMethod(const google::protobuf::MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, google::protobuf::Closure * done);
    void response(CallEntry *entry);
protected:
    void writeMessage(google::protobuf::Message* m);
    void callMethodThreaded(const google::protobuf::MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, google::protobuf::Closure * done);
    google::protobuf::Message *fillMessage(google::protobuf::Message *type,bool timeout);
    void requestServiceDescriptor(google::protobuf::Closure *cb,google::protobuf::RpcController *ctrl);
    void run();

};
};
#endif // TWOWAYSTREAM_H


