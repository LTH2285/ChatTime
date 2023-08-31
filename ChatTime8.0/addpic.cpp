#include "addpic.h"
#include "ui_addpic.h"
#include "home.h"
#include <QBuffer>
#include "protocol.h"
#include "user.h"

extern User user;
extern QString hostIp;
extern int hostPort;

addPic::addPic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPic)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
    //获取可用摄像头设备并输出在控制台
    QList<QCameraInfo> infos = QCameraInfo::availableCameras();
    qDebug() << infos.value(0).deviceName() << ":" <<infos.value(0).description();
    QString camera = infos.value(0).deviceName();
    qDebug() << camera;
    //显示摄像头
    ca =new QCamera(camera.toUtf8() ,this);
    ui->camera->show();

    QCameraViewfinder *v2 = new QCameraViewfinder(ui->camera);
    v2->resize(ui->camera->size());
    ca->setViewfinder(v2);
    v2->show();
    ca->start();
}

addPic::~addPic()
{
    delete ui;
}

void addPic::on_addPictureButton_clicked()
{
    QString path = "/home/ben/桌面/picture/myImage.jpg";
    capture = new QCameraImageCapture(ca);
    capture->capture(path);
    QMessageBox::information(this, "成功", "成功拍照");
    QImage *image = new QImage(path);
    QImage out = image->scaled(800,450,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    out.save(path, "JPG", 25);
    QImage *in = new QImage(path);


    if (tcpSocket) {
        tcpSocket->disconnectFromHost(); // 断开当前连接
        delete tcpSocket;
        tcpSocket = nullptr;
    }
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

    //ip:host
    QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;

    if(!tcpSocket->waitForConnected(3000))
       QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);

    else
    {
        if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
            qDebug() << "addpic喽";
            //服务器连接成功
            Protocol sendinfo;
            sendinfo.function_name = "addPic";
            sendinfo.userid = user.id;

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            in->save(&buffer, "JPEG");

            qDebug() << "谁的图图呢？" << sendinfo.userid;

            sendinfo.ba = byteArray;
            qDebug() << "什么图图呢？" << sendinfo.ba;
            sendinfo.sendStructData(tcpSocket,sendinfo);

            connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                qDebug() << "俺的图图呢？";

                Protocol recvinfo;
                recvinfo.recvStructData(tcpSocket,recvinfo);

                if(recvinfo.status==1)
                {
                   qDebug() << "存储成功！";
                }
                else
                {
                    qDebug() << "defeat";
                }
            });
        }
        else {
             QMessageBox::warning(this, "Warning!", "无法连接到服务器", QMessageBox::Yes);
        }
    }
    tcpSocket->disconnectFromHost();
}
