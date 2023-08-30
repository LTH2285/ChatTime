#include "protocol.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

Protocol::Protocol(){
    function_name = "func";
    username = "username";
    userip = "userip";
    userpwd = "userpwd";
    userport = 0;
    userid = 0;
    friendname = "friendname";
    friendip = "friendip";
    friendport = 0;
    friendid = 0;
    serverip = "serverip";
    serverport = 0;
    msg  = "msg";
    status = 999;
}

void Protocol::sendStructData(QTcpSocket* tcpSocket,Protocol procedure_info){

    QByteArray procedure_data;
    QDataStream procedure(&procedure_data, QIODevice::WriteOnly);
    procedure.setVersion(QDataStream::Qt_5_12);

    procedure<<procedure_info;

    if(tcpSocket->write(procedure_data)>0){
        tcpSocket->flush();
        qDebug() << procedure_info.function_name<<"sendsucces";
    }
}

void Protocol::recvStructData(QTcpSocket* tcpSocket, Protocol& procedure_info){

        //qDebug() << "daodilaimeilai";

        QByteArray datagram;

        while (tcpSocket->bytesAvailable() > 0) {
            qDebug() << "读到了！";
            datagram.append(tcpSocket->readAll());
        }

        QDataStream stream(datagram);

        stream >> procedure_info;

       // qDebug() << procedure_info.function_name << procedure_info.username << procedure_info.userid << procedure_info.userpwd ;

}

