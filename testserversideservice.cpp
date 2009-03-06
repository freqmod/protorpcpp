/*
* Copyright (C) 2009 Frederik M.J. Vestre
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "twowaystream.h"
#include "simplerpccontroller.h"
#include "testserversideservice.h"
#include "responsewaiter.h"
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
    response->set_responsecode(request->printer());
    done->Run();
}
int main(int argc,const char** argv){
    //protorpc::TwoWayStream *srv=new protorpc::TwoWayStream(testfifo,new TestServerSideService(),false);
    //srv->setOutputDevice(testofifo);
    //srv->start();
    //srv->wait();
    if(argc>1){
        QTcpSocket *soc= new QTcpSocket();
        soc->connectToHost(QHostAddress::LocalHost,1238);
        soc->waitForConnected(-1);
        protorpc::TwoWayStream *tws=new protorpc::TwoWayStream(soc,NULL,true,NULL);
        protorpc::SimpleRpcController *ctrl=new protorpc::SimpleRpcController();
        prttst::Exprintserver_Stub service(tws);
        prttst::Testarg *req=new prttst::Testarg();
        prttst::Testresp *resp=new prttst::Testresp();
        req->set_printer("Suppegjok");
        protorpc::ResponseWaiter *waiter= new protorpc::ResponseWaiter((QObject*)tws,(QObject*)ctrl,resp);
        printf("Call\n");
        service.argresp(ctrl,req,resp,waiter->getClosure());
        delete req;
        if(waiter->wait(1500)==NULL){
            if(ctrl->Failed()){
                printf("Call failed:%s\n\n",ctrl->ErrorText().c_str());
            }else{
                tws->cancelMethodCall(resp);
            }
            printf("Call timed out\n");
        }else{
            printf("PSTW>%s<\n",resp->responsecode().c_str());
        }
        tws->shutdown(true);
        tws->wait();
        delete resp;
        delete tws;
    }else{
        protorpc::SocketServer *socsrv= new protorpc::SocketServer(new TestServerSideService(),QHostAddress(QString("0.0.0.0")),1238);
        //socsrv->setShutDownOnDisconnect(true);
        socsrv->start();
        socsrv->wait();
    }
    return 0;
}
