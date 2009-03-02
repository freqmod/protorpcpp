#include "twowaystream.h"
#include "Message.pb.h"
#include "prototools.h"
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "callentry.h"
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
    if(autostart)
        start();
}
TwoWayStream::~TwoWayStream(){
        delete idev;
}
void TwoWayStream::start(){
        if(!connected){
                connected=true;
                ((QThread*) this)->start();
        }
}

void TwoWayStream::callMethod(const MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, Closure * done){
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
void TwoWayStream::callMethodThreaded(const MethodDescriptor * method, google::protobuf::RpcController * controller, const google::protobuf::Message * request, google::protobuf::Message * response, Closure * done) {
    QMutexLocker mtxlck(&streamlock);
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
  sizebuf[0]=bs&0x000000FF;
  sizebuf[1]=bs&0x0000FF00;
  sizebuf[2]=bs&0x00FF0000;
  sizebuf[3]=bs&0xFF000000;
  if(odev->write(sizebuf,4)<0)
      throw -1;
  if(odev->write(serializeToByteArray(m))<0)
      throw -1;
  odev->waitForBytesWritten(10);
  printf("Wrote message\n");
}
void TwoWayStream::requestServiceDescriptor(google::protobuf::Closure *cb,google::protobuf::RpcController *ctrl){
    QMutexLocker mtxlck(&streamlock); 	
    ctrl->SetFailed("Not supported yet");
    return;
}
/**Read a Message of type from the device*/
google::protobuf::Message *TwoWayStream::fillMessage(google::protobuf::Message *type,bool timeout){
  char bs[4];
  uint32_t msglen;
  printf("Waitmsg\n");
  if(idev->bytesAvailable()<0){
      msleep(250);
      return NULL;
  }
  uint32_t red=idev->read(bs,4);
  printf("Red%d\n",red);
  if(red<4)
    return NULL;
  //msglen=(bs[0]<<24)|(bs[1]<<16)|(bs[2]<<8)|(bs[3]<<24);
  msglen=(bs[0]<<24)|(bs[1]<<16)|(bs[2]<<8)|(bs[3]);
  printf("Gotmsg%d, %d\n",red,msglen);
  if(msglen<=0){
    delete(type);
    return NULL;
  }
  if(timeout && !idev->waitForReadyRead(timeout)){
      delete(type);
      return NULL;
  }
  printf("SRM\n");
  QByteArray msgbuf=idev->read(msglen);
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
                inmsg=(protorpc::Message*)fillMessage(protorpc::Message().New(),false);
                if(inmsg==NULL){
                    if(idev->isOpen())
                        continue;
                }
                if(inmsg==NULL||inmsg->type()==protorpc::DISCONNECT){//disconnected by stream
                    connected=false;
                    printf("Disc\n");
                    break;
                }
                printf("Recmsg:%d\n",inmsg->type());
                if (inmsg->type()==protorpc::REQUEST) {
                        printf("Grq\n");
                        method=NULL;
                        if(service==NULL){//send not implemented
                                streamlock.lock();
                                protorpc::Message *resp=protorpc::Message().New();
                                resp->set_type(protorpc::RESPONSE_NOT_IMPLEMENTED);
                                resp->set_id(inmsg->id());
                                writeMessage(resp);
                                delete resp;
                                streamlock.unlock();
                        } else{
                                printf("Req>%s<\n",inmsg->name().c_str());
                                streamlock.lock();
                                if((method=service->GetDescriptor()->FindMethodByName(inmsg->name()))==NULL){
                                        printf("mnf\n");
                                        protorpc::Message *resp=protorpc::Message().New();
                                        resp->set_type(protorpc::RESPONSE_NOT_IMPLEMENTED);
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
                                        service->CallMethod(method, controller, request,rspmsg,rspcls);
                                }
                                printf("Sq\n");
                        }
                }else if (inmsg->type()==protorpc::DESCRIPTOR_REQUEST) {
                    streamlock.lock();
                    protorpc::Message *resp=protorpc::Message().New();
                    resp->set_type(protorpc::RESPONSE_NOT_IMPLEMENTED);
                    resp->set_id(inmsg->id());
                    writeMessage(resp);
                    delete resp;
                    streamlock.unlock();
                }else if (inmsg->type()==protorpc::RESPONSE) {
                    if (currentCalls.contains(inmsg->id())) {
                            msg = (currentCalls.value(inmsg->id()));
                            msg.resp->ParseFromString(inmsg->buffer());
                            msg.callback->Run();
                            currentCalls.remove(inmsg->id());
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
        }
	  cleanup();
}
void TwoWayStream::cleanup(){
    connected=false;
    streamlock.lock();
    initcond.wakeAll();;//make sure no caller is waiting for an init signal that will never come
    streamlock.unlock();
    if(shutdownCallback!=NULL)
            shutdownCallback->Run();
    emit channelBroken();
}
/**
 * Shut down the server
 * @param closeStreams - close the iostreams?
 */
void TwoWayStream::shutdown(bool closeStreams){
    if(connected){
         streamlock.lock();
        protorpc::Message *resp=protorpc::Message().New();
        resp->set_type(protorpc::DISCONNECT);
        writeMessage(resp);
        delete resp;
        streamlock.unlock();
        connected=false;
        if(closeStreams){
            idev->close();
            if(idev!=odev)
                odev->close();
        }
        if(shutdownCallback!=NULL)
                shutdownCallback->Run();
        emit channelBroken();
    }
}
/**
  * Private: Called to signal that a response is recieved, may dissappear at any moment
  */
void TwoWayStream::response(StreamCallbackInfo *info){//Integer id, Object param,RpcController ctrl) {
    QMutexLocker mtxlck(&streamlock);
    if(service==NULL){
        delete info;
        return;
    }
    uint32_t id=info->id;
    if (!info->ctr->Failed()) {// response
        protorpc::Message rspbld = protorpc::Message();
        rspbld.set_type(protorpc::RESPONSE);
        rspbld.set_id(id);
        rspbld.set_buffer(info->msg->SerializeAsString());
        writeMessage(&rspbld);

    } else  {// canceled
        if(info->ctr->Failed()){
            protorpc::Message rspbld = protorpc::Message();
            rspbld.set_type(protorpc::RESPONSE_CANCEL);
            rspbld.set_id(info->id);
            writeMessage(&rspbld);
        }else{
            protorpc::Message rspbld = protorpc::Message();
            rspbld.set_type(protorpc::RESPONSE_CANCEL);
            rspbld.set_id(id);
            rspbld.set_buffer(info->ctr->ErrorText());
            writeMessage(&rspbld);
        }
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
