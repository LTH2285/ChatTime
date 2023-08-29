#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QWidget>

namespace Ui {
class fileOperation;
}

class fileOperation : public QWidget
{
    Q_OBJECT

public:
    explicit fileOperation(QWidget *parent = nullptr);
    ~fileOperation();

    void sendData();
private slots:
    void on_sendBtn_clicked();

    void on_cancelBtn_clicked();

    void on_selectFileBtn_clicked();

private:
    Ui::fileOperation *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QFile file;
    QTimer timer;
    QString fileName;
    quint64 fileSize;
    quint64 sendSize;
};

#endif // FILEOPERATION_H
