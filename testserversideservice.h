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
