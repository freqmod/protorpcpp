#include "socketserver.h"
namespace protorpc{
SocketServer::SocketServer(google::protobuf::Service* toServe,QHostAddress addr,uint16_t port)
{
    service=toServe;
    this->addr=addr;
    this->prt=port;
    running=false;
//    prepared=false;
}
SocketServer::~SocketServer(){
    shutdown(false);
    delete tsr;
}
#if 0
QTcpServer *SocketServer::getTcpServer(){
    printf("Wfc\n");
    while(!prepared){
        soclock.lock();
        socwait.wait(&soclock);
        soclock.unlock();
    }
    printf("Retsrv\n");
    return tsr;
}
/**
 * Start this server socket. The server socket should be set up before calling this method.
 */
void SocketServer::start(){
    if(!running&&!prepared){
        running=true;
        prepared=true;
        soclock.lock();
        socwait.wakeAll();
        soclock.unlock();
    }
}
#endif
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
        soclock.lock();
        socwait.wait(&soclock);
        soclock.unlock();
        //FIXME:interrupt thread
        TwoWayStream* s;
        foreach(s,streamServers){
           s->shutdown(closeStreams);
        }
        tsr->close();
        soclock.unlock();
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
        soclock.lock();
        tsr=new QTcpServer(NULL);
        tsr->listen(addr,prt);
        while(running){
                soclock.unlock();
                //client=ssc.accept();
                gotConnection=tsr->waitForNewConnection(250);
                if(gotConnection){

                    client=tsr->nextPendingConnection();
                    TwoWayStream *ss=new TwoWayStream(client,service,true,google::protobuf::NewCallback(shutdownCallback,this,false));
                    streamServers.insert(ss);
                }
                soclock.lock();
        }
        socwait.wakeAll();
        soclock.unlock();
        running=false;
}
void SocketServer::shutdownCallback(SocketServer *on,bool parameter) {
        if(on->shutdownOnDisconnect)
                on->shutdown(parameter);
}
};

