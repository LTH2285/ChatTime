#ifndef AIPAINT_H
#define AIPAINT_H

#include <QNetworkReply>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
//get access token
const QString baiduTokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";
const QString client_id = "pq8TvE8U7iakIhQ19dq179uw";
const QString client_secret = "nVxhGF1YnNxp0Gyo9lIANmtMQofGZjcd";
namespace Ui {
class AiPaint;
}

class AiPaint : public QWidget
{
    Q_OBJECT

public:
    explicit AiPaint(QWidget *parent = nullptr);
    ~AiPaint();
    QString getJsonValue(QByteArray ba, QString key);

    void getReply(QNetworkReply *reply);
    void sendRequestAndQueryResult(QString accessToken);
    void queryTaskResult(QString taskId, QString accessToken);
    void loadImageFromURLAndSetToLabel(QString imageUrl);
private slots:
    void on_sendBtn_clicked();

private:
    Ui::AiPaint *ui;
};

#endif // AIPAINT_H
