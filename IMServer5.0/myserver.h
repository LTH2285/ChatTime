#ifndef MYSERVER_H
#define MYSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QDateTime>
#include <QSqlQuery>
#include <protocol.h>

const int M = 20;//设置20个通信口

QT_BEGIN_NAMESPACE
namespace Ui { class myServer; }
QT_END_NAMESPACE

class myServer : public QMainWindow
{
    Q_OBJECT

public:
    myServer(QWidget *parent = nullptr);
    ~myServer();
    void onMyConnect();
    void login(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void registeruser(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void addFriend(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void createChat(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void sendMessage(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void chatHistory(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void viewFriends(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void logout(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void wantSendFile(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void resetstep1(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void resetstep2(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void getinfo(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void writephoto(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void readphoto(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void viewFriApp(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void processFriApp(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void createGroup(QTcpSocket* tcpSocket[],Protocol recvinfo);
    void chatHistory_Group(QTcpSocket* tcpSocket[],Protocol recvinfo);
   // void viewFriApp(QTcpSocket* tcpSocket[],Protocol recvinfo);

private:
    Ui::myServer *ui;
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket[M];
    QSqlDatabase db;
    QSqlQuery sqlquery;

};
#endif // MYSERVER_H
