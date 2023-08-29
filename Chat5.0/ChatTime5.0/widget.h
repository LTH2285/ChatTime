     #ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


private slots:
    void on_loginBtn_clicked();

    void on_regosterBtn_clicked();

    void on_idInput_editingFinished();

    void on_passwordInput_editingFinished();



    void on_forgetBtn_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
};
#endif // WIDGET_H
