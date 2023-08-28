#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(const int sendid, const int recvid, const QString sendusername, const QString recvusername, QWidget *parent = nullptr);
    ~Chat();

    void inserEmojiIntoLineEdit(QString emoji);
private slots:
    void on_pushButton_clicked();

    void on_chatSendBtn_clicked();

    void on_chatFileBtn_clicked();

    void on_emojiBtn_clicked();

private:
    Ui::Chat *ui;
    int m_sendid;
    int m_recvid;
    QString m_sendusername;
    QString m_recvusername;
};

#endif // CHAT_H
