#include "aipaint.h"
#include "http.h"
#include "ui_aipaint.h"

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QDebug>
#include <QUrlQuery>
#include <QJsonArray>
#include <QPixmap>
#include <QNetworkReply>
#include <QTimer>
#include <QTimer>


AiPaint::AiPaint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AiPaint)
{
    ui->setupUi(this);
}

AiPaint::~AiPaint()
{
    delete ui;
}

void AiPaint::on_sendBtn_clicked()
{
    //get access token
    QString sender = ui->constraint->text();

    QString tokenUrl = QString(baiduTokenUrl).arg(client_id).arg(client_secret);

    QMap<QString, QString> header;
    header.insert(QString("Content-Type"), QString("application/json"));

    QByteArray requestData; //send content
    QByteArray replyData; //back

    Http m_http;
    bool result = m_http.post_sync(tokenUrl, header, requestData, replyData);

    if(result){

        QString key = "access_token";
        QString accessToken = getJsonValue(replyData, key);
        qDebug()<<accessToken;

        sendRequestAndQueryResult(accessToken);
    }
    else{
        this->close();
    }

    //function





//    QString Url="http://api.qingyunke.com/api.php?key=free&appid=0&msg="+sender;
//        QUrl url;
//        url.setUrl(Url);
//        QNetworkRequest request(url);
//        QNetworkAccessManager *manager=new QNetworkAccessManager(this);
//        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getReply(QNetworkReply*)));
//        manager->get(request);

}
//QString AiPaint::getJsonValue(QByteArray ba, QString key) {
//    QJsonParseError parseError;
//    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &parseError);

//    // 检查是否成功解析 JSON
//    if (parseError.error == QJsonParseError::NoError) {
//        if (jsonDocument.isObject()) {
//            QJsonObject jsonObj = jsonDocument.object();

//            // 检查是否有 "data" 对象
//            if (jsonObj.contains("data") && jsonObj.value("data").isObject()) {
//                QJsonObject dataObj = jsonObj.value("data").toObject();

//                // 从 "data" 对象中获取 "taskId"
//                if (dataObj.contains(key)) {
//                    return QString::number(dataObj.value(key).toInt()); // 转换为 QString
//                }
//            }
//        }
//    }

//    return QString(); // 返回空字符串，或您可以选择返回适当的错误消息
//}

QString AiPaint::getJsonValue(QByteArray ba, QString key) {
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &parseError);

    // 检查是否成功解析 JSON
    if (parseError.error == QJsonParseError::NoError) {
        if (jsonDocument.isObject()) {
            QJsonObject jsonObj = jsonDocument.object();

            // 直接从顶级 JSON 对象中获取 key 对应的值
            if (jsonObj.contains(key)) {
                QJsonValue value = jsonObj.value(key);
                if (value.isString()) {
                    return value.toString();
                } else if (value.isDouble()) {
                    return QString::number(value.toInt());
                }
            }
        }
    }

    return QString(); // 返回空字符串，或您可以选择返回适当的错误消息
}
void AiPaint::sendRequestAndQueryResult(QString accessToken) {
    // Step 1: Prepare the URL for the request
    QString apiUrl = "https://aip.baidubce.com/rpc/2.0/ernievilg/v1/txt2img";
    QUrl requestUrl(apiUrl);
    QUrlQuery query;
    query.addQueryItem("access_token", accessToken);
    requestUrl.setQuery(query);

    // Step 2: Prepare headers
    QMap<QString, QString> header;
    header.insert("Content-Type", "application/json");

    // Step 3: Prepare the request body
    QJsonObject jsonBody;
    jsonBody.insert("text", ui->constraint->text());  // Assuming the text from UI
    jsonBody.insert("resolution", "1024*1024");
    jsonBody.insert("style", "二次元");
    jsonBody.insert("num", 1);
    QByteArray requestData = QJsonDocument(jsonBody).toJson();

    // Step 4: Send the request
    QByteArray replyData;
    Http m_http;
    bool result = m_http.post_sync(requestUrl.toString(), header, requestData, replyData);

    if(result) {
        // Extract taskId from the response
        QJsonDocument jsonResponse = QJsonDocument::fromJson(replyData);
        QJsonObject jsonObject = jsonResponse.object();
        QString taskId = QString::number(jsonObject["data"].toObject()["taskId"].toInt());

        // Query Task Result
        queryTaskResult(taskId, accessToken);
    } else {
        qDebug() << "Request failed!";
    }
}

void AiPaint::queryTaskResult(QString taskId, QString accessToken) {

    // Prepare the URL for the query
    QString apiUrl = "https://aip.baidubce.com/rpc/2.0/ernievilg/v1/getImg";
    QUrl requestUrl(apiUrl);
    QUrlQuery query;
    query.addQueryItem("access_token", accessToken);
    requestUrl.setQuery(query);

    // Prepare headers
    QMap<QString, QString> header;
    header.insert("Content-Type", "application/json");

    // Prepare the request body for the query
    QJsonObject jsonBody;
    jsonBody.insert("taskId", taskId);
    QByteArray requestData = QJsonDocument(jsonBody).toJson();

    // Send the query
    QByteArray replyData;
    Http m_http;
    bool result = m_http.post_sync(requestUrl.toString(), header, requestData, replyData);

    if(result) {
        // Process the query result
        QJsonDocument jsonResponse = QJsonDocument::fromJson(replyData);
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject dataObject = jsonObject["data"].toObject();

        int status = dataObject["status"].toInt();

        if(status == 0) { // If task is not complete
                   // Wait for 10 seconds and query again
                   QTimer::singleShot(5000, [this, taskId, accessToken]() {
                       queryTaskResult(taskId, accessToken);
                   });
                   return; // Important! Stop processing this result since we're going to query again
        }

        QString style = dataObject["style"].toString();
        QString text = dataObject["text"].toString();


        QString createTime = dataObject["createTime"].toString();
        QString img = dataObject["img"].toString();
        QString waiting = dataObject["waiting"].toString();




        qDebug() << "Style:" << style;
        qDebug() << "Text:" << text;
        qDebug() << "Status:" << status;
        qDebug() << "Creation Time:" << createTime;
        qDebug() << "Image URL:" << img;
        qDebug() << "Waiting Time:" << waiting;

        loadImageFromURLAndSetToLabel(img);

        QJsonArray imgUrlArray = dataObject["imgUrls"].toArray();
        for(int i = 0; i < imgUrlArray.size(); ++i) {
            QJsonObject imgUrlObject = imgUrlArray[i].toObject();
            QString imageUrl = imgUrlObject["image"].toString();
            qDebug() << "Image from Array:" << imageUrl;
        }
    } else {
        qDebug() << "Query failed!";
    }
}

void AiPaint::loadImageFromURLAndSetToLabel(QString imageUrl) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);

            // Assuming ui->imageLabel is your QLabel
            ui->picLabel->setPixmap(pixmap);
            ui->picLabel->setScaledContents(true);
        } else {
            qDebug() << "Error while downloading image:" << reply->errorString();
        }
        reply->deleteLater();
    });

    manager->get(QNetworkRequest(QUrl(imageUrl)));
}

void AiPaint::getReply(QNetworkReply *reply){
    QString data=reply->readAll();//读取回话信息
    QString tuling_get_ans;
       qDebug()<<"这是返回的结果 "+data;
       QJsonParseError json_error;
       //一定要toUtf8 否则会出错！
       QJsonDocument json=QJsonDocument::fromJson(data.toUtf8(),&json_error);
       //判断有没有错
       if(json_error.error==QJsonParseError::NoError)
       {
           if(json.isObject())
           {
               QJsonObject jsonObj=json.object();
               //取到text内容
               if(jsonObj.contains("content"))
               {
                   qDebug()<<"成功获取text";
                   QJsonValue text_value=jsonObj.take("content");

                   if(text_value.isString())
                   {
                       tuling_get_ans=text_value.toVariant().toString();
                       tuling_get_ans.replace("br","\n");
                       qDebug()<<"!!!返回的ans:"<<tuling_get_ans;
                   }
               }
           }
       }
       reply->deleteLater();
       //ui->AiChatHistory->append("AI: " + tuling_get_ans);
       //ui->AiSendInput->clear();


}

