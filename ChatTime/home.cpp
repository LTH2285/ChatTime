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
#include "aipaint.h"


QString hostip = "127.0.0.1";
int hosthost = 8888;

QList<QString> contactList = {"fuyu","wangzhihe","luoyang"};


Home::Home(const int &userid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket();

    m_userid = userid;

    Shadow *sha = new Shadow;
    sha->applyToWidget(ui->friendList);

    Home::CreateFriendList();
    ui->onlineNumber->setText(QString("welcome "+ QString::number(m_userid) + " now %1 users active").arg(contactList.length()));

}

Home::~Home()
{
    delete ui;
}
void Home::StartChat(){
    QListWidgetItem *selectedItem = ui->friendList->currentItem();

    if(selectedItem){
        int userid = selectedItem->text().trimmed().toInt();
        this->close();

        Chat *chat = new Chat(m_userid, userid, "sendusername", "recvusername");
        chat->show();


    }
    else{
        QMessageBox::warning(this, "Warning", "Please select a contact",QMessageBox::Yes);
    }
}
void Home::CreateFriendList(){
    //link server
    ui->friendList->addItems(contactList);
}

void Home::on_logoutBtn_clicked()
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
    else{
        Protocol sendinfo;
        sendinfo.function_name = "logout";
        sendinfo.userid=m_userid;
        // sendinfo.userid = 1;
        qDebug() << "logout的userid" <<sendinfo.userid;

        sendinfo.sendStructData(tcpSocket, sendinfo);
        connect(tcpSocket, &QTcpSocket::readyRead,[=](){
            Protocol recvinfo;
            recvinfo.recvStructData(tcpSocket,recvinfo);
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
    Aichat *w = new Aichat(m_userid);
    w->show();

}

void Home::on_friendList_itemDoubleClicked(QListWidgetItem *item)
{
    StartChat();
}

void Home::on_groupChatBtn_clicked()
{

}

void Home::on_aipaint_clicked()
{
    AiPaint *h = new AiPaint;
    h->show();
}
