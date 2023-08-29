#include "home.h"
#include "ui_home.h"
#include <QListWidget>
#include "QListWidgetItem"
#include "widget.h"
#include "QMessageBox"
#include "chat.h"
#include "aichat.h"
#include "uishadow.h"
#include <user.h>
#include <protocol.h>
#include <QInputDialog>
#include "friendrequest.h"

extern User user;
extern User fri;

QString hostIp = "127.0.0.1";
int hostPort = 8888;

QList<QString> contactList = {"fuyu","wangzhihe","luoyang"};

Home::Home(const int &userid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
//    m_userid = userid;
    user.id = userid;

    Shadow *sha = new Shadow;
    sha->applyToWidget(ui->friendList);

    Home::CreateFriendList();
    ui->onlineNumber->setText(QString("welcome "+ QString::number(userid) + " now %1 users active").arg(contactList.length()));

}

Home::~Home()
{
    delete ui;
}

void Home::StartChat(){
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp,hostPort);
    if(!tcpSocket->waitForConnected(3000))
    {
        //连接失败
        QMessageBox::warning(this,"Warning","Network Error",QMessageBox::Yes);
        //连接失败退回登录界面
        this->close();
        user.isonline = false;
        Widget *wid = new Widget();
        wid->show();
    }
    else
    {
        //服务器连接成功
        //建立信息头,发送给socket
        Protocol info;
        info.function_name="createChat";
        info.userid = user.id;
        QList<QString> parts = ui->friendList->currentItem()->text().split("#");
        qDebug()<<"friend id"<<parts[1];
        info.friendid = parts[1].toInt();
        fri.id = parts[1].toInt();
        info.sendStructData(tcpSocket,info);

        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
           Protocol ansInfo;//读取所有socket中的所有信息备用
           ansInfo.recvStructData(tcpSocket,ansInfo);
           QString ansType = ansInfo.function_name;

           if(ansInfo.function_name == QString("createChat_ok"))
               //
           {
               //服务器说ok
               QListWidgetItem *selectedItem = ui->friendList->currentItem();
               //打开对话框
               if(selectedItem){
                   QString contactName = selectedItem->text();
                   Chat *chat = new Chat();
                   chat->show();}
           }
           else
           {
               //服务器说error
                 QMessageBox::warning(this, "Warning!", "服务器错误", QMessageBox::Yes);
           }

        });

    }

//    QListWidgetItem *selectedItem = ui->friendList->currentItem();

//    if(selectedItem){
//        int userid = selectedItem->text().trimmed().toInt();
//        this->close();

//        Chat *chat = new Chat(m_userid, userid, "sendusername", "recvusername");
//        chat->show();


//    }
//    else{
//        QMessageBox::warning(this, "Warning", "Please select a contact",QMessageBox::Yes);
//    }
}

//好友列表显示
void Home::CreateFriendList(){
    //link server
    ui->friendList->clear();
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp,hostPort);//链接服务器
    if(!tcpSocket->waitForConnected(3000))
    {
       QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    else
    {
        //服务器连接成功
        Protocol sendinfo;
        sendinfo.function_name = "viewFriends";
        sendinfo.userid= user.id;

        sendinfo.sendStructData(tcpSocket,sendinfo);
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){

            Protocol recvinfo;
            recvinfo.recvStructData(tcpSocket,recvinfo);
            if(recvinfo.status==-1)
            {//未找到好友
              //输出未找到
                qDebug()<<"no friend found";
                ui->onlineNumber->setText(QString::number(0));
            }
            else
            {
                QList<QString> contactList = recvinfo.sql;
                qDebug()<<"friend list"<<recvinfo.sql;

                int friendnum = recvinfo.status;
                ui->friendList->addItems(contactList);

                ui->onlineNumber->setText(QString::number(friendnum));
            }
        });
    }

}

void Home::on_logoutBtn_clicked()
{
    if (tcpSocket) {
        tcpSocket->disconnectFromHost(); // 断开当前连接
        delete tcpSocket;
        tcpSocket = nullptr;
    }

    tcpSocket = new QTcpSocket();

    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

    QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    else{
        Protocol sendinfo;
        sendinfo.function_name = "logout";
        sendinfo.userid=user.id;
        // sendinfo.userid = 1;
        qDebug() << "logout的userid" <<sendinfo.userid;

        sendinfo.sendStructData(tcpSocket, sendinfo);
        connect(tcpSocket, &QTcpSocket::readyRead,[=](){
            Protocol recvinfo;
            qDebug() << recvinfo.function_name << recvinfo.status;
            recvinfo.recvStructData(tcpSocket,recvinfo);
            qDebug() << recvinfo.function_name << recvinfo.status;
            if(recvinfo.status == 1){
                qDebug()<<"logout successfully";
                this->close();
                Widget *login = new Widget;
                login->show();
            }
            else{
                QMessageBox::warning(this, "Warning", "Logout error", QMessageBox::Yes);
            }
        });
    }
}

void Home::on_startChatBtn_clicked()
{
    StartChat();
}

void Home::on_AiChatStartBtn_clicked()
{
    this->close();
    Aichat *w = new Aichat(user.id);
    w->show();

}

void Home::on_friendList_itemDoubleClicked(QListWidgetItem *item)
{
    StartChat();
}

//显示好友申请界面
void Home::on_addFriendBtn_clicked()
{
    FriendRequest *friReq = new FriendRequest(user.id);
    friReq->show();
}
