#ifndef RECVFILECLIENT_H
#define RECVFILECLIENT_H


#include <QTcpSocket>
#include <QFile>

class recvFileClient : public QObject {
  Q_OBJECT

public:
  explicit recvFileClient(QObject* parent = nullptr);
  void connectToServer(QString ip, quint16 port);

signals:
  void fileReceived(QString filePath);
  void progressUpdated(int percent);

private slots:
  void startReceiving();
  void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
  void Receivedfile();

private:
  QTcpSocket* socket;
  QString savePath;
  QFile* file;
};
#endif // RECVFILECLIENT_H
