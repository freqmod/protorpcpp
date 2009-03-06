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

#include "prototools.h"
#include <google/protobuf/message.h>

QByteArray serializeToByteArray(google::protobuf::Message *msg){
        QByteArray ra;
        ra.resize(msg->ByteSize());
        msg->SerializeToArray(ra.data(),ra.size());
        return ra;
}
QByteArray bytewiseXOR(QByteArray a1,QByteArray a2){
    QByteArray ret;
    uint16_t i,cnt=a1.count()<a2.count()?a1.count():a2.count();
    ret.resize(cnt);
    for(i=0;i<cnt;i++){
        ret[i]=(a1.at(i)^a2.at(i));
    }
    return ret;
}
QByteArray fromStdString(std::string str){
    return QByteArray(str.c_str(),str.length());
}
void writelen(QAbstractSocket *soc,uint32_t len){
    soc->write((char*)&len,4);
}
void sendMsg(QAbstractSocket* soc,google::protobuf::Message *msg){
    writelen(soc,msg->ByteSize());
    soc->write(serializeToByteArray(msg));
    soc->waitForBytesWritten();
}
std::string toStdStringUtf8(QString s){
    QByteArray u8 = s.toUtf8();
    std::string ret =std::string(u8.constData(),u8.count());
    return ret;
}
QString fromStdStringUtf8(std::string s){
    return QString::fromUtf8(s.c_str(),s.length());
}
