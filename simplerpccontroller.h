#ifndef SIMPLERPCCONTROLLER_H
#define SIMPLERPCCONTROLLER_H

#include <QObject>
#include <QSet>
#include <google/protobuf/service.h>
namespace protorpc{
class SimpleRpcController : public QObject, google::protobuf::RpcController
{
    Q_OBJECT;
public:
    SimpleRpcController();
    //client side
    std::string ErrorText();
    bool Failed();
    void startCancel();
    void Reset();
    //server side
    void setFailed(std::string reason);
    bool IsCanceled();
    void notifyOnCancel(Closure *cb);
    //extensions
signals:
    void methodFailed(SimpleRpcController *ctrl,std::string reason);
    void methodCanceled(SimpleRpcController *ctrl);
protected:
    std::String reason;
    boolean hasFailed;
    boolean canceled;
    QSet<Closure*> cancelListeners;
};
}
//#include "simplerpccontroller.moc"
#endif // SIMPLERPCCONTROLLER_H

