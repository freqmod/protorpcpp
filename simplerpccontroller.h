#ifndef SIMPLERPCCONTROLLER_H
#define SIMPLERPCCONTROLLER_H

#include <QObject>
#include <QSet>
#include <google/protobuf/service.h>
namespace protorpc{
class SimpleRpcController : public QObject, public google::protobuf::RpcController
{
    Q_OBJECT;
public:
    SimpleRpcController();
    //client side
    std::string ErrorText() const;
    bool Failed() const;
    void StartCancel();
    void Reset();
    //server side
    void SetFailed(const std::string &reason);
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure *cb);
    //extensions
signals:
    void methodFailed(SimpleRpcController *ctrl,std::string reason);
    void methodCanceled(SimpleRpcController *ctrl);
protected:
    std::string reason;
    bool hasFailed;
    bool canceled;
    QSet<google::protobuf::Closure*> cancelListeners;
};
}
//#include "simplerpccontroller.moc"
#endif // SIMPLERPCCONTROLLER_H

