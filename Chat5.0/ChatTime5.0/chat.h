#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(const int sendid, const int recvid, const QString sendusername, const QString recvusername, QWidget *parent = nullptr);
    QTimer *timer;
    ~Chat();
    Chat(QWidget *parent = nullptr);

    void inserEmojiIntoLineEdit(QString emoji);



private slots:
    void on_pushButton_clicked();

    void on_chatSendBtn_clicked();

    void on_emojiBtn_clicked();

    void getChatHistory();

    void on_sendfile_clicked();

    void on_recvfile_clicked();

    void on_screenshot_clicked();

private:
    Ui::Chat *ui;
    QTcpSocket* tcpSocket;
    QString lastUpdateTime;
};

#endif // CHAT_H
