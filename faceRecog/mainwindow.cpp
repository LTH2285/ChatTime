#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "togray.h"

#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPixmap>

#include <QMessageBox>

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //截取用户打卡时的图片
        capture = new QCameraImageCapture(ca);
        QDateTime time = QDateTime::currentDateTime();//获取当前时间
        QString nameseed = time.toString("yy-MM-dd hh-mm-ss");//将当前时间格式化，注意：时分秒之间不能用:连接，因为文件命名时不可以使用:
        capture->capture("/home/liutinghan_1120210576/Desktop/picture/"+ nameseed +".jpg");
        QMessageBox::information(this, "成功", "成功拍照");
        QImage *image = new QImage("/home/liutinghan_1120210576/Desktop/picture/lth1.jpg");

//    //截取用户登录时的图片，截取后再运行main中的随机森林
//        capture = new QCameraImageCapture(ca);
//        capture->capture("/home/liutinghan_1120210576/Desktop/picture/login.jpg");
//        QMessageBox::information(this, "成功", "成功拍照");
//        QImage *image = new QImage("/home/liutinghan_1120210576/Desktop/picture/login.jpg");
//        if(image->width()>800)
//        {
//            QImage out = image->scaled(800,450,Qt::KeepAspectRatio, Qt::SmoothTransformation);
//            bool flag = out.save("/home/liutinghan_1120210576/Desktop/picture/login_small.jpg", "JPG", 25);
////            qDebug() << flag;
//        }
}
