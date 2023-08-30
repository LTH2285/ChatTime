#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//摄像头
#include <QCameraImageCapture>
#include <QCamera>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    //创建两个摄像头对象
    QCamera *ca;
    QCameraImageCapture *capture;
};
#endif // MAINWINDOW_H
