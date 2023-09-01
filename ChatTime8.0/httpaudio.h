#ifndef HTTPAUDIO_H
#define HTTPAUDIO_H

#include <QObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>

class HttpAudio : public QObject
{
    Q_OBJECT
public:
    explicit HttpAudio(QObject *parent = nullptr);

    bool post_sync(QString Url, QMap<QString, QString> header, QByteArray requestData, QByteArray &replyData);


signals:

};

#endif // HTTPAUDIO_H
