#include "friendrequest.h"
#include "ui_friendrequest.h"
#include "home.h"
#include "ui_home.h"
#include <QListWidget>
#include <QInputDialog>
#include "QListWidgetItem"
#include "widget.h"
#include "QMessageBox"
#include "chat.h"
#include "aichat.h"
#include "uishadow.h"
#include "protocol.h"

extern QString hostIp;
extern int hostPort;
extern User user;
extern User fri;

FriendRequest::FriendRequest(int userid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendRequest)

{
    ui->setupUi(this);
    m_userid = userid;
    viewFriApp();
}

FriendRequest::~FriendRequest()
{
    delete ui;
}

void FriendRequest::on_pushButton_clicked()
{
    tcpSocket->abort();
    tcpSocket->connectToHost(hostIp, hostPort);//链接服务器
    QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }


}

void FriendRequest::viewFriApp(){
    //清空好友申请列表
    ui->friAppList->clear();
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp,hostPort);//链接服务器
    if(!tcpSocket->waitForConnected(3000))
    {
       QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    else
    {
        Protocol sendinfo;
        sendinfo.function_name = "viewFriApp";
        sendinfo.userid= user.id;

        sendinfo.sendStructData(tcpSocket,sendinfo);
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            Protocol recvinfo;
            recvinfo.recvStructData(tcpSocket,recvinfo);
            if(recvinfo.status==-1)
            {//未找到好友申请
              //输出未找到
                qDebug()<<"no friend application found";

            }
            else
            {
                QList<QString> friAppList = recvinfo.sql;
                qDebug()<<"friend application list"<<recvinfo.sql;

                int friendAppNum = recvinfo.status;
                ui->friAppList->addItems(friAppList);

            }
        });


    }
}

void FriendRequest::on_addfriendBtn_clicked()
{
    bool ok;
    int addFriendId = QInputDialog::getText(this,tr("add friends"),tr("please enter friends' user id"),QLineEdit::Normal,0,&ok).toInt();
    if(ok && addFriendId!=0)
    {
        //接收到不为空的发送好友申请请求
        if(addFriendId != user.id)
        {
            //保证好友id与本用户id不同
            tcpSocket = new QTcpSocket();
            tcpSocket->abort();
            tcpSocket->connectToHost(hostIp,hostPort);//link server
            if(!tcpSocket->waitForConnected(3000))
            {
                //连接失败的处理
                QMessageBox::warning(this,"Warning:","Net Error",QMessageBox::Yes);
                this->close();
                user.isonline=false;
                Widget *wid = new Widget();
                wid->show();
            }
            else
            {
                //连接服务器成功
                //此处应根据消息头编写发送好友申请的代码
                Protocol info;
                info.function_name = "addFriend";
                info.userid = user.id;
                info.friendid = addFriendId;
                qDebug()<<"friend id"<<info.friendid;

                //info.friendid=3;
                info.sendStructData(tcpSocket,info);

//                QString addInfo = QString("addFriend|%1|%2").arg(user.id).arg(addFriendname);

//                tcpSocket->write(addInfo.toUtf8());
//                tcpSocket->flush();
                connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                Protocol ansInfo;
                ansInfo.recvStructData(tcpSocket,ansInfo);
                qDebug()<<"发送好友申请的服务器应答"<<ansInfo.function_name;
                if(ansInfo.function_name=="addFriend_error")
                {
                    QMessageBox::warning(this,"Warning:","查询不到用户信息!",QMessageBox::Yes);
                }
                else if(ansInfo.function_name=="addFriend_already")
                {
                    QMessageBox::warning(this,"Warning:","你们已经是好友了",QMessageBox::Yes);
                }
                else if(ansInfo.function_name=="addFriend_ok")
                {
                    QMessageBox::information(this,tr("Note"),tr("发送申请成功"));
                }
            });
            }


        }
        else
        {
            QMessageBox::warning(this, "Warning!", "Can't send friend application to yourself", QMessageBox::Yes);
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning!", "好友请求不能为空", QMessageBox::Yes);
    }
}

void FriendRequest::on_exitBtn_clicked()
{

}

void FriendRequest::on_friAppList_itemDoubleClicked(QListWidgetItem *item)
{
    processFriApp();
}

void FriendRequest::processFriApp()
{
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();
    tcpSocket->connectToHost(hostIp,hostPort);
    Protocol reqInfo;
    QList<QString> parts = ui->friAppList->currentItem()->text().split(":");
    qDebug()<<"正在处理的好友请求"<<parts[0];
    reqInfo.friendid = parts[0].toInt();
    reqInfo.userid = user.id;
    reqInfo.function_name = "processFriApp";
    if(!tcpSocket->waitForConnected(3000))
    {
        QMessageBox::warning(this,"Warning!","Network error",QMessageBox::Yes);
    }
    else
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this,"Question","是否同意?",QMessageBox::Yes|QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            reqInfo.status = 1;
        }
        else if(ret == QMessageBox::No)
        {
            reqInfo.status = -1;
        }
    }
    //connect(tcpSocket,&QTcpSocket::readyRead,[=](){});
    reqInfo.sendStructData(tcpSocket,reqInfo);
    //viewFriApp();//刷新一次好友申请edit

}
