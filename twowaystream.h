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

#ifndef TWOWAYSTREAM_H
#define TWOWAYSTREAM_H

#include <Qt/QtCore>
#include <Qt/QtNetwork>
#include <callentry.h>
#include <streamcallbackinfo.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>

namespace protorpc{
class TwoWayStream : public QThread, public google::protobuf::RpcChannel
{
    Q_OBJECT
protected:
    QIODevice *idev;
    QIODevice *odev;
    QMutex *streamlock;
    QMutex devlock;
    bool connected,stopped;
    QWaitCondition initcond;
    google::protobuf::Closure* shutdownCallback;
private:
    int timeout;
    google::protobuf::Service *service;
    uint32_t callnum;
    bool spawnCallers;

    QHash<uint32_t,CallEntry> currentCalls;
public:
    TwoWayStream(QIODevice *dev,google::protobuf::Service* srv,bool autostart=false,google::protobuf::Closure *shutdownClosure=NULL);
    ~TwoWayStream();
    //void callMethod(const google::protobuf::MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, google::protobuf::Closure * done);
    void CallMethod(const google::protobuf::MethodDescriptor* method, google::protobuf::RpcController* controller, const google::protobuf::Message* request, google::protobuf::Message* response, google::protobuf::Closure* done);
    void response(StreamCallbackInfo *entry);
    void start();
    void setInputDevice(QIODevice *idev);
    /*Not owned by stream, must be deleted if set !=idev*/
    void setOutputDevice(QIODevice *odev);
    void shutdown(bool closeStreams);
    bool cancelMethodCall(google::protobuf::Message* response);
protected:
    void writeMessage(google::protobuf::Message* m);
    void callMethodThreaded(const google::protobuf::MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, google::protobuf::Closure * done);
    google::protobuf::Message *fillMessage(google::protobuf::Message *type);
    void requestServiceDescriptor(google::protobuf::Closure *cb,google::protobuf::RpcController *ctrl);
    void run();
    void cleanup();
signals:
    void channelBroken(google::protobuf::RpcChannel *chan);
};
};
//#include "twowaystream.moc"
#endif // TWOWAYSTREAM_H


