#ifndef PROTOTOOLS_H
#define PROTOTOOLS_H
#include <Qt/QtCore>
#include <Qt/QtNetwork>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

QByteArray serializeToByteArray(google::protobuf::Message *msg);
inline void parseFromByteArray(google::protobuf::Message *msg,QByteArray ary){
  msg->ParseFromArray(ary.constData(),ary.size());
}
QByteArray bytewiseXOR(QByteArray a1,QByteArray a2);
QByteArray fromStdString(std::string str);
void writelen(QAbstractSocket *soc,uint32_t len);
void sendMsg(QAbstractSocket* soc,google::protobuf::Message *msg);
std::string toStdStringUtf8(QString s);
QString fromStdStringUtf8(std::string s);

#endif // PROTOTOOLS_H
