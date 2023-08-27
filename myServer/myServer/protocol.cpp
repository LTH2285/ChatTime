#include "protocol.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

Protocol::Protocol(){

}

void Protocol::sendStructData(QTcpSocket* tcpSocket,Protocol procedure_info){

    QByteArray procedure_data;
    QDataStream procedure(&procedure_data, QIODevice::WriteOnly);
    procedure.setVersion(QDataStream::Qt_5_12);

    procedure<<procedure_info;
//    qDebug()<<procedure_data;
    if(tcpSocket->write(procedure_data)>0){
        tcpSocket->flush();
        qDebug() << procedure_info.status<<"success???";
    }
}

void Protocol::recvStructData(QTcpSocket* tcpSocket, Protocol& procedure_info){

        qDebug() << "daodilaimeilai";

        // 读取数据报及发送者信息
        QByteArray datagram;

        while (tcpSocket->bytesAvailable() > 0) {
            qDebug() << "读到了！";
            datagram.append(tcpSocket->readAll());
        }


        qDebug() << datagram;
        // 创建数据流
        QDataStream stream(datagram);
        // 反序列化数据
        stream >> procedure_info;

        qDebug() << procedure_info.function_name << procedure_info.username << procedure_info.pwd << procedure_info.port;
        datagram = tcpSocket->readAll();



}

