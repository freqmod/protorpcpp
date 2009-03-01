#ifndef TWOWAYSTREAM_H
#define TWOWAYSTREAM_H

#include <Qt/QtCore>
#include <Qt/QtNetwork>
#include <callentry.h>
#include <streamcallbackinfo.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>

namespace protorpc{
class TwoWayStream : public QThread
{
    Q_OBJECT
protected:
    QIODevice *dev;
    QMutex streamlock;
    bool connected;
    QWaitCondition initcond;
    google::protobuf::Closure* shutdownCallback;
private:
    int timeout;
    google::protobuf::Service *service;
    uint32_t callnum;
    bool spawnCallers;
    QHash<uint32_t,CallEntry> currentCalls;
public:
    TwoWayStream(QIODevice *dev,google::protobuf::Service* srv,bool autostart);
    ~TwoWayStream();
    void callMethod(const google::protobuf::MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, google::protobuf::Closure * done);
    void response(StreamCallbackInfo *entry);
    void start();
protected:
    void writeMessage(google::protobuf::Message* m);
    void callMethodThreaded(const google::protobuf::MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, google::protobuf::Closure * done);
    google::protobuf::Message *fillMessage(google::protobuf::Message *type,bool timeout);
    void requestServiceDescriptor(google::protobuf::Closure *cb,google::protobuf::RpcController *ctrl);
    void run();
    void cleanup();
signals:
        void channelBroken();
};
};
//#include "twowaystream.moc"
#endif // TWOWAYSTREAM_H


