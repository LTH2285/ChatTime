#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDateTime>
#include <protocol.h>

class Protocol{

public:
    Protocol();
    QString function_name;
    QString ip;
    QString msg;
    QString username;
    QString pwd;
    int port;
    int status;

    void sendStructData(QTcpSocket* tcpSocket,Protocol procedure_info);

    void recvStructData(QTcpSocket* tcpSocket,Protocol& procedure_info);

    friend  QDataStream & operator<< (QDataStream& stream, const Protocol& info){
        stream << info.function_name;
        stream << info.ip;
        stream << info.msg;
        stream << info.username;
        stream << info.pwd;
        stream << info.port;
        stream << info.status;
        return stream;
    }

    friend  QDataStream & operator>> (QDataStream& stream, Protocol& info){
        stream >> info.function_name;
        stream >> info.ip;
        stream >> info.msg;
        stream >> info.username;
        stream >> info.pwd;
        stream >> info.port;
        stream >> info.status;
        return stream;
    }
};


#endif // PROTOCOL_H
