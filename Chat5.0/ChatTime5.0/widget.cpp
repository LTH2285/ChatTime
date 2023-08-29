#include "widget.h"
#include "ui_widget.h"
#include "register.h"
#include "QMessageBox"
#include "home.h"
#include "uishadow.h"
#include "register.h"
#include <QtNetwork>
#include "protocol.h"
#include "resetpassword.h"

extern QString hostIp;
extern int hostPort;

// login

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket;

    Shadow *shadow = new Shadow;
    shadow->applyToWidget(ui->widget);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_loginBtn_clicked()
{

    if(ui->idInput->text()!="" && ui->passwordInput->text()!=""){
        tcpSocket->abort();
        tcpSocket->connectToHost(hostIp, hostPort);

        //ip:host
        QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
        int port = tcpSocket->peerPort();
        QString str = QString("[%1:%2]").arg(ip).arg(port);
        qDebug() << str ;

        //runouttime
        if(!tcpSocket->waitForConnected(30000))
        {
            QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        }
        //connected
        else{
            Protocol sendinfo;
            sendinfo.function_name = QString("login").trimmed();

            sendinfo.userid = ui->idInput->text().toInt();
            qDebug() << "发送的userid" <<sendinfo.userid;

            sendinfo.userpwd = ui->passwordInput->text();
            qDebug() << "发送的userpwd" <<sendinfo.userpwd;

            sendinfo.sendStructData(tcpSocket,sendinfo);
            connect(tcpSocket,&QTcpSocket::readyRead,[=](){

            Protocol recvinfo;
            recvinfo.recvStructData(tcpSocket,recvinfo);
            if(recvinfo.status==2)
            {//登陆成功
//                    user.id=QString(buffer).section("##",1,1).toInt();
                int id = ui->idInput->text().toInt();
                // user.islogin = true;
                this->close();
                Home *hom = new Home(id);
                hom->show();
                hom->welcome();
            }
            else if(recvinfo.status == -1)
            {
                QMessageBox::warning(this, "Warning!", "用户不存在", QMessageBox::Yes);
                ui->idInput->clear();
                ui->passwordInput->clear();
                ui->idInput->setFocus();

            }
            });
        }
    }

}

void Widget::on_regosterBtn_clicked()
{
    this->close();
    Register *reg = new Register();
    reg->show();
}

void Widget::on_idInput_editingFinished()
{

}

void Widget::on_passwordInput_editingFinished()
{

}


//add
void Widget::on_forgetBtn_clicked()
{
    this->close();
    ResetPassword *w = new ResetPassword(ui->idInput->text().toInt());
    qDebug() << "发送成功？" << ui->idInput->text();
    w->connectToServer();
    w->show();
    tcpSocket->disconnectFromHost();
}
