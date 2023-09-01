#ifndef CHAT_H
#define CHAT_H

#include "audio.h"

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

    void on_saveHistoryBtn_clicked();

    void on_aiAudio_pressed();

    void on_aiAudio_released();

    void closeEvent(QCloseEvent *event);

private:
    Ui::Chat *ui;
    QTcpSocket* tcpSocket;
    Audio *audio;
    QString lastUpdateTime;
};

#endif // CHAT_H
