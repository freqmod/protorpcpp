#include "socketserver.h"
namespace protorpc{
SocketServer::SocketServer(google::protobuf::Service* toServe)
{
    service=toServe;
    //tsr=QTcpServer(this);
}
QTcpServer *SocketServer::getTcpServer(){
    return &tsr;
}
/**
 * Start this server socket. The server socket should be set up before calling this method.
 */
void SocketServer::start(){
    if(!running){
        running=true;
        ((QThread*)this)->start();
    }
}

/**
 * Shut down this server socket
 * @param closeStreams - should the socket close the streams in this socket, leave to false if in doubt
 */
void SocketServer::shutdown(bool closeStreams){
    if(running){
        running=false;
        //FIXME:interrupt thread
        TwoWayStream* s;
        foreach(s,streamServers){
           s->shutdown(closeStreams);
        }
        tsr.close();
    }
}
/**
 * Should this socket shut down if a client disconnects from it?
 */
bool SocketServer::doShutDownOnDisconnect() {
        return shutdownOnDisconnect;
}
/**
 * Should this socket shut down if a client disconnects from it?
 */
void SocketServer::setShutDownOnDisconnect(bool shutDownOnDisconnect) {
        this->shutdownOnDisconnect= shutDownOnDisconnect;
}
void SocketServer::run(){
        QTcpSocket *client;
        bool gotConnection=false;
        while(running){
                //client=ssc.accept();
                gotConnection=tsr.waitForNewConnection(250);
                if(gotConnection){
                    client=tsr.nextPendingConnection();
                    TwoWayStream *ss=new TwoWayStream(client,service,google::protobuf::NewCallback(shutdownCallback,this,false));
                    streamServers.insert(ss);
                }
        }
        running=false;
}
void SocketServer::shutdownCallback(SocketServer *on,bool parameter) {
        if(on->shutdownOnDisconnect)
                on->shutdown(parameter);
}
};

