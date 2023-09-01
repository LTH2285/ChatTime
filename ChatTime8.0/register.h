#ifndef REGISTER_H
#define REGISTER_H

#include <QTcpSocket>
#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_confirmBtn_clicked();

    void on_cancelBtn_clicked();
signals:
    void registerSuccess(QString &id);

private:
    Ui::Register *ui;
    QTcpSocket *tcpSocket;
};

#endif // REGISTER_H
