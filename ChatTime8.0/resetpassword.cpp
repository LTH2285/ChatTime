#include "resetpassword.h"
#include "ui_resetpassword.h"
#include "widget.h"
#include "protocol.h"
#include <QMessageBox>
#include "home.h"

extern QString hostIp;
extern int hostPort;

ResetPassword::ResetPassword(int userid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResetPassword)
{
    ui->setupUi(this);
    m_userid = userid;

    tcpSocket = new QTcpSocket();
}

void ResetPassword::connectToServer() {
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        this->close();
        return;
    }

    qDebug() << "reset1!!!!!!!!";
    Protocol sendinfo;
    sendinfo.function_name = QString("reset1").trimmed();
    sendinfo.userid = m_userid;
    qDebug() << m_userid;
    sendinfo.sendStructData(tcpSocket, sendinfo);

    QString ip = tcpSocket->peerAddress().toString();
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;

    connect(tcpSocket, &QTcpSocket::readyRead, [=]() {
        Protocol recvinfo;
        recvinfo.recvStructData(tcpSocket, recvinfo);
        qDebug() << recvinfo.function_name << recvinfo.serverport;
        if(recvinfo.serverport == 0)
            ui->securityQuestion->setText(QString("Your security answer: Where is your hometown?"));
        else if(recvinfo.serverport == 1)
            ui->securityQuestion->setText(QString("Your security answer: What is your student ID?"));
        else if(recvinfo.serverport == 2)
            ui->securityQuestion->setText(QString("Your security answer: What is your mother's name?"));
        else
            ui->securityQuestion->setText(QString("Error: no question. Please contact project manager!!"));
    });
}

ResetPassword::~ResetPassword()
{
    delete ui;
}

void ResetPassword::on_backBtn_clicked()
{
    this->close();
    Widget *w = new Widget();
    w->show();
}

void ResetPassword::on_confirmBtn_clicked()
{
    if (tcpSocket) {
        tcpSocket->disconnectFromHost(); // 断开当前连接
        delete tcpSocket;
        tcpSocket = nullptr;
    }
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

    if (tcpSocket->waitForConnected()) {
        qDebug() << "Connected to server";

        // 在连接建立后发送数据
        Protocol sendinfo;
        sendinfo.function_name = QString("reset2").trimmed();
        sendinfo.userid = m_userid;
        sendinfo.msg = ui->answerInput->text();
        sendinfo.userpwd = ui->newPasswordInput->text();
        sendinfo.friendip = ui->repeatNewPassword->text();
        sendinfo.sendStructData(tcpSocket, sendinfo);
    } else {
        qDebug() << "Failed to connect to server";
    }
    qDebug() << "reset2!!!!!!!!";

    QString ip = tcpSocket->peerAddress().toString();
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;

    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        Protocol recvinfo;
        recvinfo.recvStructData(tcpSocket,recvinfo);

        if(recvinfo.status == 1){
            QMessageBox::information(this, "重置密码成功!", "请记好您的密码", QMessageBox::Yes);
            ui->answerInput->clear();
            ui->newPasswordInput->clear();
            ui->repeatNewPassword->clear();
            ui->answerInput->setFocus();
        }
        else if(recvinfo.status == 0){
            QMessageBox::warning(this, "Warning!", "两次密码不同", QMessageBox::Yes);
            ui->answerInput->clear();
            ui->newPasswordInput->clear();
            ui->repeatNewPassword->clear();
            ui->answerInput->setFocus();
        }
        else if(recvinfo.status == -1){
            QMessageBox::warning(this, "Warning!", "答案错误", QMessageBox::Yes);
            ui->answerInput->clear();
            ui->newPasswordInput->clear();
            ui->repeatNewPassword->clear();
            ui->answerInput->setFocus();
        }
    });
    this->close();
    Widget *cli = new Widget();
    cli->show();

}


