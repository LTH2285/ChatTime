#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTcpSocket>
#include "recvfile.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // 初始化ui
    ui->setupUi(this);
    qDebug() << "服务器主线程：" << QThread::currentThread();
    // 初始化QTcpServer
    m_s = new QTcpServer(this);

    // 连接新连接
    connect(m_s, &QTcpServer::newConnection, this, [=]()
            {
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
        }); });
}
MainWindow::~MainWindow()
{
    delete ui;
}

// 点击设置监听按钮时调用
void MainWindow::on_setListen_clicked()
{
    // 获取输入的端口号
    unsigned short port = ui->port->text().toUShort();
    // 监听指定端口
    m_s->listen(QHostAddress::Any, port);
}