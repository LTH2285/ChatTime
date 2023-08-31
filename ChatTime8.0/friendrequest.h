#ifndef FRIENDREQUEST_H
#define FRIENDREQUEST_H

#include <QWidget>
#include <QTcpSocket>
#include <QListWidgetItem>

namespace Ui {
class FriendRequest;
}

class FriendRequest : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRequest(int userid , QWidget *parent = nullptr);
    void viewFriApp();
    void processFriApp();
    ~FriendRequest();

private slots:
    void on_pushButton_clicked();

    void on_addfriendBtn_clicked();

    void on_exitBtn_clicked();

    void on_friAppList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::FriendRequest *ui;
    int m_userid;
    QTcpSocket *tcpSocket;
};

#endif // FRIENDREQUEST_H
