#include "sendfile.h"
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>
SendFile::SendFile(QObject *parent) : QObject(parent)
{
}

void SendFile::connectServer(unsigned short port, QString ip)
{
    // 创建一个QTcpSocket对象
    m_tcp = new QTcpSocket;
    // 连接服务器
    m_tcp->connectToHost(QHostAddress(ip), port);
    // 连接成功后，调用connectedOK函数
    connect(m_tcp, &QTcpSocket::connected, this, &SendFile::connectedOK);
    // 连接断开后，调用gameover函数
    connect(m_tcp, &QTcpSocket::disconnected, this, [=]()
            {
        m_tcp->close();
        m_tcp->deleteLater();
        emit gameover(); });
}

void SendFile::sendFile(QString path)
{
    // 打开文件
    QFile file(path);
    // 获取文件信息
    QFileInfo info(path);
    // 获取文件大小
    int fileSize = info.size();

    // 打开文件
    file.open(QFile::ReadOnly);

    // 定义当前发送的数据大小
    static int num = 0;
    m_tcp->write((char *)&fileSize, 4);

    // 循环发送文件
    while (!file.atEnd())
    {
        // 读取文件的一行数据
        QByteArray line = file.readLine();
        // 将读取的数据大小加到当前发送的数据大小中
        num += line.size();
        // 计算发送的百分比
        int percent = (num * 100 / fileSize);
        // 调用curPercent函数，发送发送的百分比
        emit curPercent(percent);
        // 发送文件的一行数据
        m_tcp->write(line);
    }
}