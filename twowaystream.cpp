#include "twowaystream.h"
#include "Message.pb.h"
#include "prototools.h"
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "callentry.h"
#include <string.h>
#include "streamcallbackinfo.h"
#include "simplerpccontroller.h"
#include "twowayrpccontroller.h"
#include "twowaystream.moc"
namespace protorpc{
using namespace google::protobuf;
TwoWayStream::TwoWayStream(QIODevice *dev,google::protobuf::Service* srv,bool autostart,Closure *shutdownClosure)
{
    timeout=10000;
    spawnCallers=false;
    callnum=0;
    shutdownCallback=NULL;
    this->idev=dev;
    this->odev=dev;
    this->service=srv;
    this->shutdownCallback=shutdownClosure;
    connected=false;

    streamlock=new QMutex(QMutex::Recursive);
    if(autostart)
        start();
}
TwoWayStream::~TwoWayStream(){
        delete idev;
        delete streamlock;
}
void TwoWayStream::start(){
        if(!connected){
                connected=true;
                stopped=false;
                ((QThread*) this)->start();
        }
}

//void TwoWayStream::callMethod(const MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, Closure * done){
void TwoWayStream::CallMethod(const google::protobuf::MethodDescriptor* method, google::protobuf::RpcController* controller, const google::protobuf::Message* request, google::protobuf::Message* response, google::protobuf::Closure* done){
    if(spawnCallers){
            //Class<?> paramTypes[]={MethodDescriptor.class,RpcController.class,google::protobuf::Message.class,google::protobuf::Message.class,RpcCallback.class};
/*            Object params[]={method,controller,request,responsePrototype,done};
            try {
                    ThreadTools.invokeInSeparateThread(getClass().getMethod("callMethodThreaded", paramTypes), this,params);
            } catch (SecurityException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
            } catch (NoSuchMethodException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
            }*/
    }else{
            callMethodThreaded(method, controller, request, response, done);
    }
}
bool TwoWayStream::cancelMethodCall(google::protobuf::Message* response){
    QMutexLocker mtxlck(streamlock);
    uint32_t mk=0;
    bool found=false;
    QHashIterator<uint32_t, CallEntry> i(currentCalls);
    while (i.hasNext()) {
        i.next();
        if(i.value().resp==response){
            mk=i.key();
            found=true;
            break;
        }
    }
    if(found)
        currentCalls.remove(mk);
    return found;
}
void TwoWayStream::callMethodThreaded(const MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, Closure * done) {
    QMutexLocker mtxlck(streamlock);
    try {
        protorpc::Message reqbld;
        //MessageProto.Message.Builder reqbld = google::protobuf::MessageProto.Message.newBuilder();
        reqbld.set_type(protorpc::REQUEST);
        reqbld.set_id(callnum);
        reqbld.set_name(method->name());
        reqbld.set_buffer(request->SerializeAsString());
        writeMessage(&reqbld);
        currentCalls.insert(callnum,CallEntry(controller,done,response));
        callnum++;
    } catch (int e) {
        controller->SetFailed("IOError");
    }
}
void TwoWayStream::writeMessage(google::protobuf::Message* m){
  char sizebuf[4];
  uint32_t bs=m->ByteSize();
  sizebuf[0]=bs&0xFF000000;
  sizebuf[1]=bs&0x00FF0000;
  sizebuf[2]=bs&0x0000FF00;
  sizebuf[3]=bs&0x000000FF;
  if(odev->write(sizebuf,4)<0){
      throw odev->errorString().toUtf8().constData();
  }
  if(odev->write(serializeToByteArray(m))<0)
      throw -1;
  odev->waitForBytesWritten(10);
  printf("Wrote message\n");
}
void TwoWayStream::requestServiceDescriptor(google::protobuf::Closure *cb,google::protobuf::RpcController *ctrl){
    QMutexLocker mtxlck(streamlock);
    ctrl->SetFailed("Not supported yet");
    return;
}
/**Read a Message of type from the device*/
google::protobuf::Message *TwoWayStream::fillMessage(google::protobuf::Message *type){
  char bs[4];
  uint32_t msglen;
  streamlock->lock();
  if(!connected||!idev->isOpen()){
    delete(type);
    streamlock->unlock();
    return NULL;
  }
  idev->waitForReadyRead(250);
  streamlock->unlock();
  if(idev->bytesAvailable()<1){
      msleep(250);
      delete(type);
      return NULL;
  }
  //printf("Waitmsg\n");

  uint32_t red=idev->read(bs,4);
  printf("Red%d,%s\n",red,idev->errorString().toUtf8().data());
  if(red<4){
    delete(type);
    return NULL;
  }
  //msglen=(bs[0]<<24)|(bs[1]<<16)|(bs[2]<<8)|(bs[3]<<24);
  msglen=(bs[0]<<24)|(bs[1]<<16)|(bs[2]<<8)|(bs[3]);
  if(bs[0]!=0){
      printf("Most likely got out of sync");
  }
  printf("Gotmsg%d, %d\n",red,msglen);
  if(msglen<=0){
    delete(type);
    return NULL;
  }
  uint32_t vld=0;
  printf("STRD\n");
  idev->waitForReadyRead(250);
  QByteArray msgbuf =idev->read(msglen);
  if(msgbuf.length()< msglen){
      printf("Extread%d<%d\n",msgbuf.length(),msglen);
      while(connected&&msgbuf.length()<msglen){
          idev->waitForReadyRead(250);
          QByteArray recv =idev->read(msglen-vld);        
          if(recv.length()>0)
            msgbuf.append(recv);
      }
  }
  printf("SRM\n");
  //QByteArray msgbuf=idev->read(msglen);
  printf("Red%d\n",msgbuf.length());
  parseFromByteArray(type,msgbuf);
  return type;
}

/**
  * Private: Main thread method
  */
void TwoWayStream::run() {
        const MethodDescriptor *method=NULL;
        google::protobuf::Message *request=NULL;
	SimpleRpcController *controller;
	CallEntry msg;
        protorpc::Message *inmsg=NULL;
        while (connected) {
                inmsg=(protorpc::Message*)fillMessage(protorpc::Message().New());
                if(inmsg==NULL){
                    if(idev->isOpen()){
                        printf("NISO\n");
                        continue;
                    }
                }
                if(inmsg==NULL||inmsg->type()==protorpc::DISCONNECT){//disconnected by stream
                    connected=false;
                    if(inmsg)
                        delete inmsg;
                    printf("Disc\n");
                    break;
                }
                printf("Recmsg:%d\n",inmsg->type());
                if (inmsg->type()==protorpc::REQUEST) {
                        printf("Grq\n");
                        method=NULL;
                        if(service==NULL){//send not implemented
                                streamlock->lock();
                                printf("nullsrv\n");
                                protorpc::Message *respo=protorpc::Message().New();
                                respo->set_type(protorpc::RESPONSE_NOT_IMPLEMENTED);
                                respo->set_id(inmsg->id());
                                writeMessage(respo);
                                delete respo;
                                streamlock->unlock();
                        } else{
                                printf("Req>%s<\n",inmsg->name().c_str());
                                streamlock->lock();
                                if((method=service->GetDescriptor()->FindMethodByName(inmsg->name()))==NULL){
                                        printf("mnf>%s<\n",inmsg->name().c_str());
                                        protorpc::Message *resp=protorpc::Message().New();
                                        resp->set_type(protorpc::RESPONSE_NOT_IMPLEMENTED);//this fails if the program ends executing when the thread is still running
                                        resp->set_id(inmsg->id());
                                        writeMessage(resp);
                                        delete resp;
                                }
                                if(method!=NULL){
                                        printf("mfnd\n");
                                        request = service->GetRequestPrototype(method).New();
                                        request->ParseFromString(inmsg->buffer());
                                        controller = new TwoWayRpcController(this);
                                        google::protobuf::Message *rspmsg=service->GetResponsePrototype(method).New();
                                        StreamCallbackInfo *sci=new StreamCallbackInfo(this,controller,rspmsg,inmsg->id());
                                        google::protobuf::Closure *rspcls=google::protobuf::NewCallback(StreamCallbackInfo::callhack,sci);
                                        //NewCallback(this, &Handler::FooDone, response);
                                        //void (*resp)(StreamCallbackInfo*)=response;
                                        controller->NotifyOnCancel(rspcls);
                                        printf("cm\n");
                                        streamlock->unlock();
                                        service->CallMethod(method, controller, request,rspmsg,rspcls);
                                        streamlock->lock();
                                }
                                printf("Sq\n");
                        }
                }else if (inmsg->type()==protorpc::DESCRIPTOR_REQUEST) {
                    streamlock->lock();
                    protorpc::Message *resp=protorpc::Message().New();
                    resp->set_type(protorpc::RESPONSE_NOT_IMPLEMENTED);
                    resp->set_id(inmsg->id());
                    writeMessage(resp);
                    delete resp;
                    streamlock->unlock();
                }else if (inmsg->type()==protorpc::RESPONSE) {
                    streamlock->lock();
                    if (currentCalls.contains(inmsg->id())) {
                            msg = (currentCalls.value(inmsg->id()));
                            printf("Bla:>%s<\n",inmsg->buffer().c_str());
                            msg.resp->ParseFromString(inmsg->buffer());//A sigsegv here most probably means that the response is deleted and has left a dangling pointer.
                            currentCalls.remove(inmsg->id());
                            streamlock->unlock();
                            msg.callback->Run();
                    }else{
                        streamlock->unlock();
                    }
                }else if (inmsg->type()==protorpc::RESPONSE_CANCEL) {
                    if (currentCalls.contains(inmsg->id())) {
                            msg = (currentCalls.value(inmsg->id()));//get controller
                            msg.ctrl->StartCancel();
                            currentCalls.remove(inmsg->id());
                    }
                }else if (inmsg->type()==protorpc::RESPONSE_FAILED) {
                        if (currentCalls.contains(inmsg->id())) {
                                msg = currentCalls.value(inmsg->id());//get controller
                                msg.ctrl->SetFailed(inmsg->buffer());
                                currentCalls.remove(inmsg->id());
                        }
                }else if (inmsg->type()==protorpc::RESPONSE_NOT_IMPLEMENTED) {
                        if (currentCalls.contains(inmsg->id())) {
                                msg = currentCalls.value(inmsg->id());//get controller
                                msg.ctrl->SetFailed("Not implemented by peer");
                                currentCalls.remove(inmsg->id());
                        }
                }else if (inmsg->type()==protorpc::DESCRIPTOR_RESPONSE/*&&descriptorRequestController!=NULL&&gotDescriptorCallback!=NULL*/) {
                    //NOT IMPLEMENTED/DON'T CARE
                }else{//empty buffer
                        //in.skip(in.available());
                }
                delete inmsg;
        }
	  cleanup();
          streamlock->lock();
          stopped=true;
          streamlock->unlock();
}
void TwoWayStream::cleanup(){
    connected=false;
    //streamlock->lock();
    //initcond.wakeAll();;//make sure no caller is waiting for an init signal that will never come
    //streamlock->unlock();
    if(shutdownCallback!=NULL)
            shutdownCallback->Run();
    emit channelBroken(this);
}
/**
 * Shut down the server
 * @param closeStreams - close the iostreams?
 */
void TwoWayStream::shutdown(bool closeStreams){
    if(connected){
        streamlock->lock();
        protorpc::Message *resp=protorpc::Message().New();
        resp->set_type(protorpc::DISCONNECT);
        try{
            writeMessage(resp);
        }catch(const char* e){
            //do nothing as it is is not able to write to a closed socket etc.
        }
        delete resp;

        connected=false;
#if 0 //don''t
        //wait for thread to stop
        streamlock->lock();
        while(stopped==false){
            streamlock->unlock();
            msleep(100);
            streamlock->lock();
        }
        streamlock->unlock();
#endif

        if(closeStreams){
            idev->close();
            if(idev!=odev)
                odev->close();
        }
        streamlock->unlock();
        //wait until thread is stopped
        emit channelBroken(this);//may create problems whith queue signals etc. if shutdownCallback deletes this object
        if(shutdownCallback!=NULL)
                shutdownCallback->Run();

    }
}
/**
  * Private: Called to signal that a response is recieved, may dissappear at any moment
  */
void TwoWayStream::response(StreamCallbackInfo *info){//Integer id, Object param,RpcController ctrl) {
    printf("TWRecresp\n");
    QMutexLocker mtxlck(streamlock);
    printf("Glk\n");
    if(service==NULL){
        printf("Noservice\n");
        delete info;
        return;
    }
    uint32_t id=info->id;
    if (!info->ctr->Failed()) {// response
        printf("Sresp\n");
        protorpc::Message rspbld = protorpc::Message();
        rspbld.set_type(protorpc::RESPONSE);
        rspbld.set_id(id);
        rspbld.set_buffer(info->msg->SerializeAsString());
        try{
            writeMessage(&rspbld);
        }catch(const char* e){
            shutdown(true);
        }

    } else  {// canceled
        printf("cncl\n");

        protorpc::Message rspbld = protorpc::Message();
        rspbld.set_type(protorpc::RESPONSE_CANCEL);
        rspbld.set_id(id);
        rspbld.set_buffer(info->ctr->ErrorText());
        writeMessage(&rspbld);
    }
    currentCalls.remove(info->id);
    delete info->msg;
    delete info->ctr;

}
void TwoWayStream::setInputDevice(QIODevice *idev){
    this->idev=idev;
}
void TwoWayStream::setOutputDevice(QIODevice *odev){
    this->odev=odev;
}



}
