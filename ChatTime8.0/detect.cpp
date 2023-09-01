#include "detect.h"
#include "ui_detect.h"
#include "togray.h"

#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPixmap>

#include <QMessageBox>

#include <QDateTime>
#include "addpic.h"
#include "protocol.h"
#include "user.h"
#include <QBuffer>
#include "randomforest.h"
#include "home.h"

extern User user;
extern QString hostIp;
extern int hostPort;

detect::detect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::detect)
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

detect::~detect()
{
    delete ui;
}

void detect::on_pushButton_clicked()
{
//    QString path = "/home/ben/桌面/picture/login1.jpg";
    QString path = "/home/liutinghan_1120210576/Desktop/picture/login1.jpg";
    capture = new QCameraImageCapture(ca);
    capture->capture(path);
    QMessageBox::information(this, "成功", "成功拍照");
//        QImage *image = new QImage("login1.jpg");
    QImage *image = new QImage(path);
    if(image->width()!=160)
    {
        QImage out = image->scaled(160,90,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        bool flag = out.save(path, "JPG", 20);
//    qDebug() << flag;
    }

    randomForest rf;
    QVector<QImage> allPic;


    QImage *shot = new QImage(path);

//    本地无socket连接
    int id = -1;
    id = ui->IDlineEdit->text().toUInt();
    if(id <= 0 )
    {
        QMessageBox::information(this, "未输入ID", "请先输入ID");
        return;
    }
    QString begin;
    begin.setNum(id);
    QDir dir("/home/liutinghan_1120210576/Desktop/picture");
    QFileInfoList files = dir.entryInfoList(QDir::Files);

    for(int i=0; i<files.size(); i++)
    {
        if(files[i].fileName().startsWith(begin))
        {
            QFile *file = new QFile(files[i].absoluteFilePath());
            qDebug() << file->fileName();
            QImage image(file->fileName());
            free(file);
            allPic.append(image);
        }
    }


    bool so = rf.makeDecision(allPic,*shot);
    if(so){
        this->close();
        Home *hom = new Home(id);
        hom->show();
        hom->welcome();
    }
    else
    {
        QMessageBox::information(this, "失败", "匹配失败");
//        free(&allPic);
    }


//    QVector<QImage> allPic;
//    QImage *img = new QImage("/home/liutinghan_1120210576/Desktop/picture/1_1.jpg");
//    allPic.append(*img);
//    bool so = rf.makeDecision(allPic,*shot);
//    qDebug() << so;
//    //                so = true;
//    if(so){
//        this->close();
//        Home *hom = new Home(id);
//        hom->show();
//        hom->welcome();
//    }
//    else
//    {
//        QMessageBox::information(this, "失败", "匹配失败");
//        free(&allPic);
//    }

//    free(img);




//    if (tcpSocket) {
//        tcpSocket->disconnectFromHost(); // 断开当前连接
//        delete tcpSocket;
//        tcpSocket = nullptr;
//    }
//    tcpSocket = new QTcpSocket();
//    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

//    //ip:host
//    QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
//    int port = tcpSocket->peerPort();
//    QString str = QString("[%1:%2]").arg(ip).arg(port);
//    qDebug() << str ;

//    if(!tcpSocket->waitForConnected(3000))
//       QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);

//    else
//    {
//        if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
//            qDebug() << "看看实力";
//            //服务器连接成功
//            Protocol sendinfo;
//            sendinfo.function_name = "detect";
//            sendinfo.userid= user.id;

//            QByteArray byteArray;
//            QBuffer buffer(&byteArray);
//            buffer.open(QIODevice::WriteOnly);

//            sendinfo.ba = byteArray;
//            sendinfo.sendStructData(tcpSocket,sendinfo);

//            connect(tcpSocket,&QTcpSocket::readyRead,[=](){
//                qDebug() << "果真吗";

//                Protocol recvinfo;
//                recvinfo.recvStructData(tcpSocket,recvinfo);
//                randomForest rf;

//                QImage *shot = new QImage(path);

//                QList<QByteArray> allImage = recvinfo.friendIcon;

//                QVector<QImage> allPic;

//                int id = -1;
//                id = ui->IDlineEdit->text().toUInt();
//                if(id == -1)
//                {
//                    QMessageBox::information(this, "未输入ID", "请先输入ID");
//                    return;
//                }
//                QString begin;
//                begin.setNum(id);
//                QDir dir("/home/liutinghan_1120210576/Desktop/picture");
//                QFileInfoList files = dir.entryInfoList(QDir::Files);

//                for(int i=0; i<files.size(); i++)
//                {
//                    if(files[i].fileName().startsWith(begin))
//                    {
//                        QFile *file = new QFile(files[i].absoluteFilePath());
//                        qDebug() << file->fileName();
//                        QImage image(file->fileName());
//                        free(file);
//                        allPic.append(image);
//                    }
//                }


//                bool so = rf.makeDecision(allPic,*shot);
//                if(so){
//                    this->close();
//                    Home *hom = new Home(id);
//                    hom->show();
//                    hom->welcome();
//                }
//                else
//                {
//                    QMessageBox::information(this, "失败", "匹配失败");
//                    free(&allPic);
//                }
//            });
//        }
//        else {
//            QMessageBox::warning(this, "Warning!", "无法连接到服务器", QMessageBox::Yes);
//       }
//    }
//    tcpSocket->disconnectFromHost();
    free(shot);
}



