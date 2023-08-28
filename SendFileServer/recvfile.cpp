/*
 * @Author: LTH
 * @Date: 2023-08-27 22:39:44
 * @LastEditTime: 2023-08-28 09:12:15
 * @FilePath: \Mini-term-assignments-for-junior-year\SendFileServer\recvfile.cpp
 * @Description:
 * Copyright (c) 2023 by LTH, All Rights Reserved.
 */
#include "recvfile.h"
#include <QFile>
#include <QTcpSocket>
#include "mainwindow.h"
#include "ui_mainwindow.h"

RecvFile::RecvFile(QTcpSocket *tcp, QObject *parent) : QThread(parent)
{
    m_tcp = tcp;
}

void RecvFile::run()
{
    // 创建文件对象
    QFile *file = new QFile("/Users/physicsmile/Desktop/codes/build-SendFileServer-Desktop_Qt_5_9_9_clang_64bit-Debug/recv.txt");
    // 打开文件
    file->open(QFile::WriteOnly);
    // 接收数据
    connect(m_tcp, &QTcpSocket::readyRead, this, [=]()
            {
        static int count=0;
        static int total=0;
        QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
        if(count==0)
        {
            client->read((char*)&total,4);
        }
        // 读剩余数据
        QByteArray all= client->readAll();
        count+=all.size();

     file->write(all);



        // 获取消息内容
        QString senderName="1";
        QString receiverName="2";
        if(!clientMap.contains(senderName)) {
            clientMap[senderName] = client;
        }
            // 转发消息到目标客户端
            if(clientMap.contains(receiverName)) {
//                QString forwardedMessage = senderName + "|" + receiverName + "|" + message;
                clientMap[receiverName]->write(all);
            }



        if(count==total)
        {
            m_tcp->close();
            m_tcp->deleteLater();
            file->close();
            file->deleteLater();
            emit over();
        } });
    // 进入事件循环
    exec();
}

//        QStringList parts = QString(all).split('|');

//        if(parts.count() >= 3) {
//            QString senderName = parts[0];  // 获取发送者名称
//            QString receiverName = parts[1]; // 获取接收者名称

//            // 如果映射中还没有该客户端，添加它
//            if(!clientMap.contains(senderName)) {
//                clientMap[senderName] = client;
//            }

//            // 获取消息内容
//            QString message = parts[2];