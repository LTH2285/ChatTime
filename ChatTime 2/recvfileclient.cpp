#include "recvfileclient.h"

recvFileClient::recvFileClient(QObject* parent) : QObject(parent) {
  socket = new QTcpSocket(this);
}

void recvFileClient::connectToServer(QString ip, quint16 port) {
  socket->connectToHost(ip, port);

  connect(socket, &QTcpSocket::connected, this, &recvFileClient::startReceiving);
}

void recvFileClient::startReceiving() {
  // 连接上后准备接收文件

  // 获取保存路径
  savePath = "received.txt";

  // 打开文件
  file = new QFile(savePath);
  file->open(QIODevice::WriteOnly);

  // 连接readyRead信号和相应的slot
  connect(socket, &QTcpSocket::readyRead, this, &recvFileClient::updateProgress);

  // 连接信号处理文件接收完成
  connect(socket, &QTcpSocket::disconnected, this, &recvFileClient::fileReceived);
}

// 更新进度的slot
void recvFileClient::updateProgress(qint64 bytesReceived, qint64 bytesTotal) {
  // 计算进度百分比
  int percent = bytesReceived * 100 / bytesTotal;

  // 发射progressUpdated信号通知界面
  emit progressUpdated(percent);

  // 将接收到的数据写入文件
  file->write(socket->readAll());
}

// 文件接收完毕的slot
void recvFileClient::Receivedfile() {
  // 关闭文件
  file->close();

  // 发送文件接收完毕的信号
  emit fileReceived(savePath);
}
