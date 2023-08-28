#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTcpSocket>

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    //explicit Chat(const QString &contactname, QWidget *parent = nullptr);
    explicit Home(const int &userid, QWidget *parent = nullptr);
    ~Home();
    void CreateFriendList();
    void StartChat();

private slots:
    void on_logoutBtn_clicked();


    void on_startChatBtn_clicked();


    void on_AiChatStartBtn_clicked();

    void on_friendList_itemDoubleClicked(QListWidgetItem *item);

    void on_groupChatBtn_clicked();

    void on_aipaint_clicked();

private:
    Ui::Home *ui;
    int m_userid;
    QTcpSocket *tcpSocket;

};

#endif // HOME_H
