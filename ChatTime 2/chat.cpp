/*
 * @Author: LTH
 * @Date: 2023-08-27 22:39:44
 * @LastEditTime: 2023-08-28 09:32:54
 * @FilePath: \Mini-term-assignments-for-junior-year\ChatTime 2\chat.cpp
 * @Description:
 * Copyright (c) 2023 by LTH, All Rights Reserved.
 */
#include "chat.h"
#include "ui_chat.h"
#include <QApplication>
#include <QListWidget>
#include "home.h"
#include <QApplication>
#include <QListWidget>
#include "fileoperation.h"
#include "facecharacter.h"
#include <QDebug>

Chat::Chat(const QString &contactname, QWidget *parent) : QWidget(parent),
                                                          ui(new Ui::Chat),
                                                          m_contactname(contactname)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Chat with %1").arg(m_contactname));
}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_pushButton_clicked()
{
    this->close();
    Home *home = new Home;
    home->show();
}

void Chat::on_chatSendBtn_clicked()
{
    // 发送消息
    QString message = ui->chatInput->text();
    if (!message.trimmed().isEmpty())
    {
        ui->chatHistory->append("I chat to " + m_contactname + " :" + message);
    }
}

void Chat::on_chatFileBtn_clicked()
{
    // 文件传输
    fileOperation *file = new fileOperation();
    file->show();
}

void Chat::on_emojiBtn_clicked()
{
    // 打开表情对话框
    FaceCharacter *w = new FaceCharacter(this);
    connect(w, &FaceCharacter::emojiSelected, this, &Chat::inserEmojiIntoLineEdit);
    w->show();
}

void Chat::inserEmojiIntoLineEdit(QString emoji)
{
    // 添加表情到输入框
    QString currentText = ui->chatInput->text();
    ui->chatInput->setText(currentText + emoji);
}