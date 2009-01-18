#ifndef TWOWAYSTREAM_H
#define TWOWAYSTREAM_H

#include <Qt/QtCore>
#include <Qt/QtNetwork>
#include <callentry.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
namespace protorpc{
class TwoWayStream : public QThead
{
protected:
    QIODevice *dev;
    QMutex streamlock=new QMutex();
    google::protobuf::Message type;
private:
    int timeout=10000;
    google::protobuf::Service *service;
    int callnum=0;
    boolean spawnCallers=false;
    QHash<CallEntry> currentCalls;
public:
    TwoWayStream(QIODevice *dev,Service* srv,boolean autostart);
protected:
    writeMessage(google::protobuf::Message m);
};
};
#endif // TWOWAYSTREAM_H


