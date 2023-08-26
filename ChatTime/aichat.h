#ifndef AICHAT_H
#define AICHAT_H

#include <QWidget>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

namespace Ui {
class Aichat;
}

class Aichat : public QWidget
{
    Q_OBJECT

public:
    explicit Aichat(QWidget *parent = nullptr);
    ~Aichat();


private slots:
    void on_AiQuitBtn_clicked();

    void on_AiChatSendBtn_clicked();

    void getReply(QNetworkReply *reply);

private:
    Ui::Aichat *ui;
};

#endif // AICHAT_H
