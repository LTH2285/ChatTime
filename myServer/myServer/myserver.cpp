#include "myserver.h"
#include "ui_myserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <protocol.h>
#include <QObject>

int currentsize;

myServer::myServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::myServer)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer();

    ui->listWidget->clear();
    ui->listWidget->insertItem(0,tr("当前无在线用户"));
    for(int i = 0; i < M; i++)
    {
        tcpSocket[i]=new QTcpSocket();
    }
    tcpServer->listen(QHostAddress::Any,8888);
    QHostAddress serverAddress = tcpServer->serverAddress();
    QString serverIp = serverAddress.toString();
    qDebug() << "服务器IP地址：" << serverIp;

    //init
    db = QSqlDatabase::addDatabase("QSQLITE");

    //判断是否建立了用户表
    db.setDatabaseName("./people.db");
    db.open();
    QSqlQuery sqlquery;
    sqlquery.exec("CREATE TABLE if not exists people (id INTEGER NOT NULL UNIQUE,name TEXT NOT NULL UNIQUE,password TEXT NOT NULL,ip TEXT,islogin INTEGER NOT NULL,PRIMARY KEY(id AUTOINCREMENT))");
    db.close();

    connect(tcpServer,&QTcpServer::newConnection,this,&myServer::onMyConnect);
}

myServer::~myServer()
{
    tcpServer->close();
    tcpServer->deleteLater();
    delete ui;
}

void myServer::onMyConnect(){

    tcpSocket[0] = tcpServer->nextPendingConnection();
    currentsize++;
    QString ip = tcpSocket[0]->peerAddress().toString().section(":",3,3);
    int port = tcpSocket[0]->peerPort();
    //预留currentsize以用作多用户同时连接所用
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;

    connect(tcpSocket[0],&QTcpSocket::readyRead,[=](){
        //读取缓冲区数据
        Protocol recvinfo;
        recvinfo.recvStructData(tcpSocket[0],recvinfo);
        qDebug() << recvinfo.username;

        if("login" == recvinfo.function_name)
            login(tcpSocket,recvinfo);
        else{
            qDebug() << "zen me shier?";
        }
    });
}

void myServer::login(QTcpSocket* tcpSocket[],Protocol recvinfo){
    Protocol sendinfo;
    //登陆
    db.setDatabaseName("./people.db");
    db.open();
    QSqlQuery sqlquery;
    sqlquery.prepare("select * from people where name = :name");

    sqlquery.bindValue(":name",recvinfo.username);
    sqlquery.exec();
    if(!sqlquery.next())
    {//未查找到该用户
        sendinfo.status = -1;
        sendinfo.sendStructData(tcpSocket[0],sendinfo);
        db.close();
    }
    else
    {//用户存在
        sendinfo.status = 0;
        int id = sqlquery.value(0).toInt();
        QString pwd = sqlquery.value(2).toString();
        if(pwd == recvinfo.pwd)
        {//登录成功
            qDebug() << "登陆成功!";
            sendinfo.status = 2;
            sendinfo.username = QString("uzi").trimmed();
            sendinfo.pwd = QString("789").trimmed();
            sendinfo.sendStructData(tcpSocket[0],sendinfo);
            sqlquery.prepare("update people set ip=:ip, islogin=1 where name = :name");
            sqlquery.bindValue(":ip",recvinfo.ip);
            sqlquery.bindValue(":name",recvinfo.username);
            sqlquery.exec();
            tcpSocket[0]->flush();

            //更新服务器界面
            ui->listWidget->clear();
            sqlquery.prepare("select * from people where islogin = 1");
            sqlquery.exec();
            if(sqlquery.next())
            {
                QString userid = sqlquery.value(0).toString();
                QString username = sqlquery.value(1).toString();
                QString userip = sqlquery.value(3).toString();
                //qDebug()<<userid;
                ui->listWidget->insertItem(0,"用户ID："+userid+",用户昵称:"+username+",用户IP:"+userip);
                int rownum = 1;
                while (sqlquery.next())
                {
                    QString userid = sqlquery.value(0).toString();
                    QString username = sqlquery.value(1).toString();
                    QString userip = sqlquery.value(3).toString();
                    ui->listWidget->insertItem(rownum,"用户ID："+userid+",用户昵称:"+username+",用户IP:"+userip);
                    rownum++;
                }
            }
            else
            {
                ui->listWidget->clear();
                ui->listWidget->insertItem(0,tr("当前无在线用户"));
            }
        }
        else
        {//密码错误
            sendinfo.msg = QString("login error!!errpwd!!").toUtf8();
            sendinfo.sendStructData(tcpSocket[0],sendinfo);
            db.close();
        }
    }
}
