#include "twowaystream.h"
#include "Message.pb.h"
#include "prototools.h"
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "callentry.h"
#include "streamcallbackinfo.h"
namespace protorpc{
using google::protobuf;
TwoWayStream::TwoWayStream()
{
}
TwoWayStream::callMethod(const MethodDescriptor * method, RpcController * controller, const Message * request, Message * response, Closure * done){
    if(spawnCallers){
            //Class<?> paramTypes[]={MethodDescriptor.class,RpcController.class,Message.class,Message.class,RpcCallback.class};
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
            callMethodThreaded(method, controller, request, responsePrototype, done);
    }
}
void TwoWayStream::callMethodThreaded(const MethodDescriptor * method, RpcController * controller, const Message * request, Message * response, Closure * done) {
    QMutexLocker mtxlck(&streamlock);
    try {
        protorpc::Message reqbld;
        //MessageProto.Message.Builder reqbld = MessageProto.Message.newBuilder();
        reqbld.set_type(MessageProto.Type.REQUEST);
        reqbld.set_id(callnum);
        reqbld.set_name(method.getName());
        reqbld.set_buffer(request.toByteString());
        writeMessage(reqbld);
        currentCalls.put(callnum, new CallEntry(ctrl,resp,msg));
        callnum++;
    } catch (IOException e) {
        controller.setFailed(e.getMessage());
    }
}
void TwoWayStream::writeMessage(Message m){
  char sizebuf[4];
  uint32_t bs=m.ByteSize();
  sizebuf[0]=bs&0x000000FF;
  sizebuf[1]=bs&0x0000FF00;
  sizebuf[2]=bs&0x00FF0000;
  sizebuf[3]=bs&0xFF000000;
  dev->write(sizebuf,4);  
  dev->write(serializeToByteArray(&m));
  dev->waitForBytesWritten(10);
}
void TwoWayStream::requestServiceDescriptor(Closure *cb,RpcController *ctrl){
    QMutexLocker mtxlck(&streamlock); 	
    ctrl->setFailed("Not supported yet");
    return;
}
/**Read a message of type from the device*/
Message *TwoWayStream::fillMessage(Message type,boolean timeout){
  char sizebuf[4];
  uint32_t msglen;
  uint red=dev->read(sizebuf,4);
  if(red<4)
    throw ; //something
  msglen=(bs[0]<<24)|(bs[1]<<16)|(bs[2]<<8)|(bs[3]<<24);   
  if(msglen<=0)
    return NULL;
  char msgbuf[msglen];
  if(timeout && !dev->waitForReadyRead(timeout))
      return NULL;
  QByteArray msgbuf;
  dev->read(msgbuf);
  return parseFromByteArray(type.New(),msgbuf);
}

/**
  * Private: Main thread method
  */
void TwoWayStream::run() {
	MethodDescriptor *method=NULL;
	Message *request=NULL;
	SimpleRpcController *controller;
	CallEntry msg;
	protorpc::Message *inmsg=NULL;
        while (connected) {
                inmsg=(protorpc::Message*)fillMessage(MessageProto.Message.getDefaultInstance(),false);
                if(inmsg==null||inmsg->type()==protorpc::DISCONNECT){//disconnected by stream
                        connected=false;
                        break;
                }
                if (inmsg->type()==protorpc::REQUEST&&service!=NULL) {
                        method=NULL;
                        if(service==NULL){//send not implemented
                                streamlock.lock();
                                protorpc::Message *resp=protorpc::Message.New();
                                resp->set_type(protorpc::RESPONSE_NOT_IMPLEMENTED);
                                resp->set_id(inmsg->id());
                                writeMessage(resp);
                                delete resp;
                                streamlock.unlock();
                        } else{
                                streamlock.lock();
                                if((method=service->GetDescriptor()->FindMethodByName(inmsg.getName()))==null){
                                        protorpc::Message *resp=protorpc::Message.New();
                                        resp->set_typesetType(protorpc::RESPONSE_NOT_IMPLEMENTED);
                                        resp->set_id(inmsg->id());
                                        writeMessage(resp);
                                        delete resp;
                                }
                                if(method!=null){
                                        request = service->GetRequestPrototype(method).New();
                                        request->ParseFromString(inmsg->buffer());
                                        controller = new TwoWayRpcController(this);
                                        controller.notifyOnCancel(google::protobuf::NewCallback(run,new StreamCallbackInfo(this,controller,inmsg->id())));
                                        service->CallMethod(method, controller, request,
                                                                google::protobuf::NewCallback(run,new StreamCallbackInfo(this,controller,inmsg->id())));
                                }
                        }
                }else if (inmsg->type()==protorpc::DESCRIPTOR_REQUEST) {
                    streamlock.lock();
                    protorpc::Message *resp=protorpc::Message.New();
                    resp->set_typesetType(protorpc::RESPONSE_NOT_IMPLEMENTED);
                    resp->set_id(inmsg->id());
                    writeMessage(resp);
                    delete resp;
                }else if (inmsg->type()==protorpc::RESPONSE) {
                    if (currentCalls.contains(inmsg.id())) {
                            msg = currentCalls.value(inmsg.id());
                            msg.resp->ParseFromString(inmsg->buffer());
                            msg.callback->Run();
                            currentCalls.remove(inmsg.id());
                    }
                }else if (inmsg->type()==protorpc::RESPONSE_CANCEL) {
                    if (currentCalls.contains(inmsg->id())) {
                            msg = currentCalls.get(inmsg->id());//get controller
                            msg.ctrl->StartCancel();
                            currentCalls.remove(inmsg->id());
                    }
                }else if (inmsg->type()==protorpc::RESPONSE_FAILED) {
                        if (currentCalls.contains(inmsg.id())) {
                                msg = currentCalls.get(inmsg->id());//get controller
                                msg.ctrl->SetFailed(inmsg.getBuffer());
                                currentCalls.remove(inmsg.id);
                        }
                }else if (inmsg->type()==protorpc::RESPONSE_NOT_IMPLEMENTED) {
                        if (currentCalls.contains(inmsg.id())) {
                                msg = currentCalls.get(inmsg->id());//get controller
                                msg.ctrl->SetFailed("Not implemented by peer");
                                currentCalls.remove(inmsg.id);
                        }
                }else if (inmsg->type()==protorpc::DESCRIPTOR_RESPONSE&&descriptorRequestController!=null&&gotDescriptorCallback!=null) {
                    //NOT IMPLEMENTED/DON'T CARE
                }else{//empty buffer
                        //in.skip(in.available());
                }
        }
	  cleanup();
}	
void cleanup(){
    connected=false;
    streamlock.lock();
    initcond.signalAll();//make sure no caller is waiting for an init signal that will never come
    streamlock.unlock();
    if(shutdownCallback!=null)
	    shutdownCallback.run(false);
    fireChannelBroken();
}

/**
  * Private: Called to signal that a response is recieved, may dissappear at any moment
  */
void TwoWayStream::run(CallEntry *entry){//Integer id, Object param,RpcController ctrl) {
    QMutexLocker mtxlck(&streamlock);
    if(service==null){
        delete entry;
        return;
    }
    uint32_t id=info->id;
    if (entry->resp!=NULL) {// response
        protorpc::Message rspbld = protorpc::Message();
        rspbld.set_type(protorpc::RESPONSE);
        rspbld.set_id(id);
        rspbld.set_buffer(entry->resp->SerializeAsString());
        writeMessage(rspbld);
    } else  {// canceled
        if(ctrl.failed()){
            protorpc::Message rspbld = protorpc::Message();
            rspbld.set_type(protorpc::RESPONSE_CANCEL);
            rspbld.set_id(id);
            writeMessage(rspbld);
        }else{
            protorpc::Message rspbld = protorpc::Message();
            rspbld.set_type(protorpc::RESPONSE_CANCEL);
            rspbld.set_id(id);
            rspbld.set_buffer(entry->ctrl->ErrorText());
            writeMessage(rspbld);
        }
    }
}



}
