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
    void register(QTcpSocket* tcpSocket[],Protocol recvinfo);

private:
    Ui::myServer *ui;
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket[M];
    QSqlDatabase db;
    QSqlQuery sqlquery;

};
#endif // MYSERVER_H
