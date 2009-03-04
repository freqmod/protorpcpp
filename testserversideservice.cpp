#include "twowaystream.h"
#include "simplerpccontroller.h"
#include "testserversideservice.h"
#include "socketserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

TestServerSideService::TestServerSideService(){
}
void TestServerSideService::resp(::google::protobuf::RpcController* controller,
                   const ::prttst::Void* request,
                   ::prttst::Testresp* response,
                   ::google::protobuf::Closure* done){
    response->set_responsecode("yeay");
    printf("Rnrspfunc\n");
    done->Run();
}
void TestServerSideService::arg(::google::protobuf::RpcController* controller,
                   const ::prttst::Testarg* request,
                   ::prttst::Void* response,
                   ::google::protobuf::Closure* done){
}
void TestServerSideService::argresp(::google::protobuf::RpcController* controller,
                   const ::prttst::Testarg* request,
                   ::prttst::Testresp* response,
                   ::google::protobuf::Closure* done){

}
int main(int argc,const char** argv){
    //protorpc::TwoWayStream *srv=new protorpc::TwoWayStream(testfifo,new TestServerSideService(),false);
    //srv->setOutputDevice(testofifo);
    //srv->start();
    //srv->wait();
    if(argc>1){
        QTcpSocket *soc= new QTcpSocket();
        soc->connectToHost(QHostAddress::LocalHost,1238);
        protorpc::TwoWayStream *tws=new protorpc::TwoWayStream(soc,NULL,true,NULL);
        protorpc::SimpleRpcController *ctrl=new protorpc::SimpleRpcController();
        prttst::Exprintserver_Stub service(tws);
        prttst::Void req();
        //Res
        service.resp(ctrl,req,waiter.Callback());
    }else{
        protorpc::SocketServer *socsrv= new protorpc::SocketServer(new TestServerSideService(),QHostAddress(QString("0.0.0.0")),1238);
        socsrv->start();
        socsrv->wait();
    }
    return 0;
}
