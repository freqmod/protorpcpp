#include "testserversideservice.h"
#include "twowaystream.h"
#include <QTcpServer>
#include <QTcpSocket>
TestServerSideService::TestServerSideService(){
}
void TestServerSideService::resp(::google::protobuf::RpcController* controller,
                   const ::prttst::Void* request,
                   ::prttst::Testresp* response,
                   ::google::protobuf::Closure* done){
    prttst::Testresp rsp;
    rsp.set_responsecode("yeay");
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
    QFile *testfifo= new QFile("test.fifo");
    testfifo->open(QFile::ReadWrite);
    protorpc::TwoWayStream *srv=new protorpc::TwoWayStream(testfifo,new TestServerSideService(),true);
    srv->wait();
    return 0;
}
