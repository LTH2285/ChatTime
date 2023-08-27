#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTcpSocket>
#include "recvfile.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"服务器主线程："<<QThread::currentThread();
    m_s=new QTcpServer(this);

    connect(m_s,&QTcpServer::newConnection,this,[=](){
//        QTcpSocket* tcp = m_s->nextPendingConnection();

        //多个客户端加入
        QTcpSocket *newClient = m_s->nextPendingConnection();
        clients.append(newClient);

        //创建子线程
        RecvFile* subThread = new RecvFile(newClient);
        subThread->start();
        connect(subThread,&RecvFile::over,this,[=](){
            subThread->exit();
            subThread->wait();
            subThread->deleteLater();
            QMessageBox::information(this,"文件接受","文件接收完毕!");
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any,port);

}
