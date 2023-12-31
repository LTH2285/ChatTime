#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QMap>
#include <QMapIterator>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>

class Http : public QObject
{
    Q_OBJECT
public:
    explicit Http(QObject *parent = nullptr);

    bool post_sync(QString Url, QMap<QString, QString> header, QByteArray requestData, QByteArray &replayData);

signals:

};

#endif // HTTP_H
