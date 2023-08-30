#include "fileoperation.h"
#include "ui_fileoperation.h"
#include "QFileDialog"

#include <QMessageBox>
#include <QTcpSocket>
#include <QTimer>

extern QString hostIp;
extern int hostPort;

fileOperation::fileOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileOperation)
{
    ui->setupUi(this);
    connect(&timer,&QTimer::timeout,[=](){
        //关闭定时器
        timer.stop();
        //发送文件
        sendData();
    });

    int sendport = 8899;//开放8899端口用于接受端连接
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,sendport);

    connect(tcpServer,&QTcpServer::newConnection,[=](){
        tcpSocket = tcpServer->nextPendingConnection();
//        QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
//        QString showmessage = ip + "已连接";
//        ui->pushButton_sendfile->setEnabled(true);
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            QByteArray buffer = tcpSocket->readAll();
            qDebug()<<buffer;
            if("send_ok" == QString(buffer)){
                this->close();
            }
        });
    });
//    ui->pushButton_sendfile->setEnabled(false);

}

fileOperation::~fileOperation()
{
    delete ui;
}

void fileOperation::on_sendBtn_clicked()
{
    //先发送文件头信息 文件名##文件大小
       QString head = QString("%1##%2").arg(fileName).arg(fileSize);
       //发送头部信息
       qint64 len = tcpSocket->write(head.toUtf8());
       //判断信息是否发送成功
       if(len > 0)
       {
           //发送文件信息，防止tcp黏包信息，定时器延时
           timer.start(20);
       }
       else
       {
           QMessageBox::information(this,"Error:","头部信息发送失败");
           file.close();
           return;
       }
}

void fileOperation::on_cancelBtn_clicked()
{
    this->close();
}

void fileOperation::on_selectFileBtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择文件","../");
    //若文件路径无效，则提示
    if(filePath.isEmpty())
    {
        QMessageBox::information(this,"Error: ",filePath+"文件路径无效");
        return;
    }
    fileName.clear();
    fileSize = 0;
    sendSize = 0;
    //获取文件信息
    QFileInfo info(filePath);
    fileName = info.fileName();
    fileSize = info.size();

    //指定文件的名称
    file.setFileName(filePath);
    //以只读方式打开文件
    bool isOK = file.open(QIODevice::ReadOnly);
    if(!isOK)
    {
        QMessageBox::information(this,"Error:","只读方式打开文件失败");
        return;
    }
    else
    {
//        提示打开文件路径
        ui->fileNameInput->setText("选择文件成功："+filePath);
        ui->selectFileBtn->setEnabled(false);
    }
}

void fileOperation::sendData()
{
    quint64 len = 0;
    do
    {
        //每次发送数据的大小
        char buf[1024] = {0};
        len = 0;
        //往文件里读数据
        len = file.read(buf,sizeof(buf));
        //发送数据，读多少，发多少
        len = tcpSocket->write(buf,len);
        sendSize += len;
    }
    while(len >0);
    QMessageBox::information(this,"Success:","发送完毕");
    //send signal to database
    this->close();
}

void fileOperation::closeEvent(QCloseEvent *event)
{
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp, hostPort);//链接服务器
    if(!tcpSocket->waitForConnected(30000))
    {
        this->close();
    }
    else
    {//服务器连接成功
//        QString loginmessage = QString("send_file_miss##%1##%2").arg(user.id).arg(otheruser.name);
//        qDebug()<<loginmessage;
//        tcpSocket->write(loginmessage.toUtf8());
//        tcpSocket->flush();
//        is_open_chatdialog = false;
    }
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpServer->disconnect();
    tcpServer->close();
}
