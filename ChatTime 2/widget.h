     #ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

    void on_usernameInput_editingFinished();

    void on_passwordInput_editingFinished();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
