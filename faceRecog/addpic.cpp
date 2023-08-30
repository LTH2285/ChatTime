#include "addpic.h"
#include "ui_addpic.h"


addPic::addPic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPic)
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

addPic::~addPic()
{
    delete ui;
}

void addPic::on_addPictureButton_clicked()
{
    QString path = "/home/liutinghan_1120210576/Desktop/picture/login.jpg";
    capture = new QCameraImageCapture(ca);
    capture->capture(path);
    QMessageBox::information(this, "成功", "成功拍照");
    QImage *image = new QImage(path);
    QImage out = image->scaled(800,450,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    out.save(path, "JPG", 25);
    QImage *in = new QImage(path);
//    连接数据库
//    Func func;
//    func::insertUserImage(ID,*in);
}
