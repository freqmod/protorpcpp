/* Copyright (C) 2009 Frederik M.J.V

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * LGPL is available on the internet at
 * http://www.gnu.org/licenses/lgpl-2.1.html and from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
*/

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
