#ifndef TESTSERVERSIDESERVICE_H
#define TESTSERVERSIDESERVICE_H
#include "test/test.pb.h"
class TestServerSideService : public prttst::Exprintserver
{
public:
    TestServerSideService();
   void resp(::google::protobuf::RpcController* controller,
                       const ::prttst::Void* request,
                       ::prttst::Testresp* response,
                       ::google::protobuf::Closure* done);
  void arg(::google::protobuf::RpcController* controller,
                       const ::prttst::Testarg* request,
                       ::prttst::Void* response,
                       ::google::protobuf::Closure* done);
  void argresp(::google::protobuf::RpcController* controller,
                       const ::prttst::Testarg* request,
                       ::prttst::Testresp* response,
                       ::google::protobuf::Closure* done);

};

#endif // TESTSERVERSIDESERVICE_H
