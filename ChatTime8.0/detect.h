#ifndef DETECT_H
#define DETECT_H

#include <QDialog>
//摄像头
#include <QCameraImageCapture>
#include <QCamera>
#include <QTcpSocket>
#include <QDir>

namespace Ui {
class detect;
}

class detect : public QDialog
{
    Q_OBJECT

public:
    explicit detect(QWidget *parent = nullptr);
    ~detect();

private slots:
    void on_pushButton_clicked();

private:
    Ui::detect *ui;
    //创建两个摄像头对象
    QCamera *ca;
    QCameraImageCapture *capture;
    QTcpSocket* tcpSocket;
};

#endif // DETECT_H


