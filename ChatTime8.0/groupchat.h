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


    void refresh_List();
private slots:
    void on_refreshBtn_clicked();

    void on_sendBtn_clicked();

    void on_Quit_clicked();

    void getChatHistory();

    void on_saveHistoryBtn_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::GroupChat *ui;
    QTcpSocket* tcpSocket;
    QString lastUpdateTime;
};

#endif // GROUPCHAT_H
