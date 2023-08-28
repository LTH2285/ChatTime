#include "friendrequest.h"
#include "ui_friendrequest.h"
#include "home.h"
#include "ui_home.h"
#include <QListWidget>
#include "QListWidgetItem"
#include "widget.h"
#include "QMessageBox"
#include "chat.h"
#include "aichat.h"
#include "uishadow.h"
#include "protocol.h"

extern QString hostip;
extern int hosthost;

FriendRequest::FriendRequest(int userid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendRequest)

{
    ui->setupUi(this);
    m_userid = userid;
}

FriendRequest::~FriendRequest()
{
    delete ui;
}

void FriendRequest::on_pushButton_clicked()
{
    tcpSocket->abort();
    tcpSocket->connectToHost(hostip, hosthost);//链接服务器
    QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }


}

void FriendRequest::on_pushButton_2_clicked()
{

}
