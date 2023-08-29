#include "http.h"
#include <QMapIterator>

Http::Http(QObject *parent) : QObject(parent)
{

}

bool Http::post_sync(QString Url, QMap<QString, QString> header, QByteArray requestData, QByteArray &replyData){

    //send object
    QNetworkAccessManager manager;

    //request object
    QNetworkRequest request;
    request.setUrl(Url);
    QMapIterator<QString, QString> it(header);
    while(it.hasNext()){
        it.next();
        request.setRawHeader(it.key().toLatin1(),it.value().toLatin1());
    }

    QNetworkReply *reply = manager.post(request, requestData);
    QEventLoop l;

    connect(reply, &QNetworkReply::finished, &l, &QEventLoop::quit);
    l.exec();

    if(reply != nullptr && reply->error() == QNetworkReply::NoError){
        replyData = reply->readAll();
        qDebug()<< replyData;
        return true;
    }
    else{
        qDebug()<< "request failed";
        return false;
    }
}
