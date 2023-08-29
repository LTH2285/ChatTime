#include "aichat.h"
#include "ui_aichat.h"
#include "home.h"
#include "QMessageBox"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Aichat::Aichat(const int userid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Aichat)
{
    ui->setupUi(this);
}

Aichat::~Aichat()
{
    delete ui;
}

void Aichat::on_AiQuitBtn_clicked()
{
    this->close();
    Home *home = new Home(userid);
    home->show();
}

void Aichat::on_AiChatSendBtn_clicked()
{

    QString sender = ui->AiSendInput->text();
    ui->AiChatHistory->append("I chat to AI: " + sender);
    if(sender == ""){
        QMessageBox::warning(this, "Warning", "Empty message is not allowed", QMessageBox::Yes);
    }
    else{
        QString Url="http://api.qingyunke.com/api.php?key=free&appid=0&msg="+ui->AiSendInput->text();
            QUrl url;
            url.setUrl(Url);
            QNetworkRequest request(url);
            QNetworkAccessManager *manager=new QNetworkAccessManager(this);
            connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getReply(QNetworkReply*)));
            manager->get(request);
    }
}
void Aichat::getReply(QNetworkReply *reply){
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
       ui->AiChatHistory->append("AI: " + tuling_get_ans);
       ui->AiSendInput->clear();


}
