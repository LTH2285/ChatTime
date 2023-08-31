#ifndef RESETPASSWORD_H
#define RESETPASSWORD_H

#include <QTcpSocket>
#include <QWidget>

namespace Ui {
class ResetPassword;
}

class ResetPassword : public QWidget
{
    Q_OBJECT

public:
    explicit ResetPassword(int userid, QWidget *parent = nullptr);
    ~ResetPassword();
    void connectToServer();

private slots:
    void on_backBtn_clicked();

    void on_confirmBtn_clicked();

private:
    Ui::ResetPassword *ui;
    int m_userid;
    QTcpSocket *tcpSocket;
};

#endif // RESETPASSWORD_H
