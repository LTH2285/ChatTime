#include "speech.h"

#include <QJsonParseError>

speech::speech()
{

}
QString speech::speechIdentify(QString fileName)
{
    QString accessToken;

    //获取access_token
    QString tokenUrl = QString(baiduTokenUrl).arg(client_id).arg(client_secret);

    QMap<QString, QString> header;
    header.insert(QString("Content-Type"), QString("audio/pcm;rate=16000"));

    QByteArray requestData;        //发送的内容
    QByteArray replyData;          //服务器返回的内容

    HttpAudio m_http;
    bool result = m_http.post_sync(tokenUrl, header, requestData, replyData);
    if (result)
    {
        QString key = "access_token";
        accessToken = getJsonValue(replyData, key);
        qDebug() << accessToken;
    }
    else
    {

    }

    //组合URl
    QString speechUrl = QString(baiduSpeechUrl).arg(QHostInfo::localHostName()).arg(accessToken);

    //把文件转换成QByteArray;
    QFile file;
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);
    requestData =file.readAll();
    file.close();

    replyData.clear();

    //再次发起请求
    result = m_http.post_sync(speechUrl, header, requestData, replyData);
    if (result)
    {
        QString key = "result";
        QString text = getJsonValue(replyData, key);
        return text;
        //qDebug() << accessToken;
    }
    else
    {
        QMessageBox::warning(NULL, "识别提示", "识别失败");
    }

    return "";
}

QString speech::getJsonValue(QByteArray ba, QString key)
{
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &parseError);
    if (parseError.error == QJsonParseError::NoError)
    {
        if (jsonDocument.isObject())
        {
            //jsonDocument转换成json对象
            QJsonObject jsonObj = jsonDocument.object();
            if (jsonObj.contains(key))
            {
                QJsonValue jsonVal = jsonObj.value(key);
                if (jsonVal.isString())     //字符串
                {
                    return jsonVal.toString();
                }
                else if (jsonVal.isArray()) //数组
                {
                    QJsonArray arr = jsonVal.toArray();    //转换成JsonArray
                    QJsonValue jv = arr.at(0);             //获取第1个元素
                    return jv.toString();
                }
            }
        }
    }

    return "";
}

