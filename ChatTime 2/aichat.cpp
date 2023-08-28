/*
 * @Author: LTH
 * @Date: 2023-08-27 22:39:44
 * @LastEditTime: 2023-08-28 09:31:43
 * @FilePath: \Mini-term-assignments-for-junior-year\ChatTime 2\aichat.cpp
 * @Description:
 * Copyright (c) 2023 by LTH, All Rights Reserved.
 */
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

Aichat::Aichat(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Aichat)
{
    // 初始化Ui
    ui->setupUi(this);
}

Aichat::~Aichat()
{
    // 销毁Ui
    delete ui;
}

void Aichat::on_AiQuitBtn_clicked()
{
    // 退出程序
    this->close();
    // 创建Home类的实例
    Home *home = new Home;
    // 显示Home类的实例
    home->show();
}

void Aichat::on_AiChatSendBtn_clicked()
{
    // 获取发送的消息
    QString sender = ui->AiSendInput->text();
    // 添加消息到历史记录
    ui->AiChatHistory->append("I chat to AI: " + sender);
    // 如果消息为空，提示用户
    if (sender == "")
    {
        QMessageBox::warning(this, "Warning", "Empty message is not allowed", QMessageBox::Yes);
    }
    // 否则，发送消息到AI
    else
    {
        // 创建URL
        QString Url = "http://api.qingyunke.com/api.php?key=free&appid=0&msg=" + ui->AiSendInput->text();
        QUrl url;
        url.setUrl(Url);
        // 创建请求
        QNetworkRequest request(url);
        // 创建网络操作管理器
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        // 连接请求
        connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(getReply(QNetworkReply *)));
        // 发送请求
        manager->get(request);
    }
}
void Aichat::getReply(QNetworkReply *reply)
{
    // 获取返回的消息
    QString data = reply->readAll();
    // 将消息转换为字符串
    QString tuling_get_ans;
    qDebug() << "这是返回的结果 " + data;
    // 将消息转换为字符串
    QJsonParseError json_error;
    QJsonDocument json = QJsonDocument::fromJson(data.toUtf8(), &json_error);
    // 如果解析失败，提示用户
    if (json_error.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject jsonObj = json.object();
            if (jsonObj.contains("content"))
            {
                qDebug() << "成功获取text";
                QJsonValue text_value = jsonObj.take("content");

                QString sender = ui->AiSendInput->text();
                ui->AiChatHistory->append("I chat to AI: " + sender);
                if (sender == "")
                    if (text_value.isString())
                    {
                        QMessageBox::warning(this, "Warning", "Empty message is not allowed", QMessageBox::Yes);
                        tuling_get_ans = text_value.toVariant().toString();
                        tuling_get_ans.replace("br", "\n");
                        qDebug() << "!!!返回的ans:" << tuling_get_ans;
                    }
                    else
                    {
                        QString Url = "http://api.qingyunke.com/api.php?key=free&appid=0&msg=" + ui->AiSendInput->text();
                        QUrl url;
                        url.setUrl(Url);
                        QNetworkRequest request(url);
                        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
                        connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(getReply(QNetworkReply *)));
                        manager->get(request);
                    }
            }
            void Aichat::getReply(QNetworkReply * reply)
            {
                QString data = reply->readAll();
                QString tuling_get_ans;
                qDebug() << "这是返回的结果 " + data;
                QJsonParseError json_error;
                QJsonDocument json = QJsonDocument::fromJson(data.toUtf8(), &json_error);
                if (json_error.error == QJsonParseError::NoError)
                {
                    if (json.isObject())
                    {
                        QJsonObject jsonObj = json.object();
                        if (jsonObj.contains("content"))
                        {
                            qDebug() << "成功获取text";
                            QJsonValue text_value = jsonObj.take("content");

                            if (text_value.isString())
                            {
                                tuling_get_ans = text_value.toVariant().toString();
                                tuling_get_ans.replace("br", "\n");
                                qDebug() << "!!!返回的ans:" << tuling_get_ans;
                            }
                            // 删除请求
                            reply->deleteLater();
                            // 添加消息到历史记录
                            ui->AiChatHistory->append("AI: " + tuling_get_ans);
                            // 清空输入框
                            ui->AiSendInput->clear();
                        }
                    }
                }
                reply->deleteLater();
                ui->AiChatHistory->append("AI: " + tuling_get_ans);
                ui->AiSendInput->clear();
            }