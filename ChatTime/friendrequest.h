#ifndef FRIENDREQUEST_H
#define FRIENDREQUEST_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class FriendRequest;
}

class FriendRequest : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRequest(int userid , QWidget *parent = nullptr);
    ~FriendRequest();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FriendRequest *ui;
    int m_userid;
    QTcpSocket *tcpSocket;
};

#endif // FRIENDREQUEST_H
