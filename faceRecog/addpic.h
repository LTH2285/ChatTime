#ifndef ADDPIC_H
#define ADDPIC_H

#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPixmap>

#include <QMessageBox>


namespace Ui {
class addPic;
}

class addPic : public QDialog
{
    Q_OBJECT

public:
    explicit addPic(QWidget *parent = nullptr);
    ~addPic();

private slots:
    void on_addPictureButton_clicked();

private:
    Ui::addPic *ui;

    //创建两个摄像头对象
    QCamera *ca;
    QCameraImageCapture *capture;
};

#endif // ADDPIC_H
