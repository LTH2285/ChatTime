#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class GroupChat;
}

class GroupChat : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChat(QWidget *parent = nullptr);
    QTimer *timer;
    ~GroupChat();


private slots:
    void on_refreshBtn_clicked();

    void on_sendBtn_clicked();

    void on_Quit_clicked();

    void getChatHistory();

private:
    Ui::GroupChat *ui;
    QTcpSocket* tcpSocket;
    QString lastUpdateTime;
};

#endif // GROUPCHAT_H
