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

    QString username;
    QString userip;
    QString userpwd;
    int userport;
    int userid;

    QString friendname;
    QString friendip;
    int friendid;
    int friendport;

    QString serverip;
    int serverport;

    QString msg;

    // 1代表操作成功,-1代表操作失败
    int status;

    // 群聊id
    int groupid;

    // 1代表在线,0代表离线
    int islogin;

    // 群聊消息,好友列表,历史记录
    QList <QString> sql;

    void sendStructData(QTcpSocket* tcpSocket,Protocol procedure_info);

    void recvStructData(QTcpSocket* tcpSocket,Protocol& procedure_info);

    friend  QDataStream & operator<< (QDataStream& stream, const Protocol& info){
        stream << info.function_name;
        stream << info.username;
        stream << info.userip;
        stream << info.userpwd;
        stream << info.userport;
        stream << info.userid;
        stream << info.friendname;
        stream << info.friendip;
        stream << info.friendport;
        stream << info.serverip;
        stream << info.serverport;
        stream << info.msg;
        stream << info.status;
        stream << info.groupid;
        stream << info.islogin;
        stream << info.sql;
        return stream;
    }

    friend  QDataStream & operator>> (QDataStream& stream, Protocol& info){
        stream >> info.function_name;
        stream >> info.username;
        stream >> info.userip;
        stream >> info.userpwd;
        stream >> info.userport;
        stream >> info.userid;
        stream >> info.friendname;
        stream >> info.friendip;
        stream >> info.friendport;
        stream >> info.serverip;
        stream >> info.serverport;
        stream >> info.msg;
        stream >> info.status;
        stream >> info.groupid;
        stream >> info.islogin;
        stream >> info.sql;
        return stream;
    }

};



#endif // PROTOCOL_H
