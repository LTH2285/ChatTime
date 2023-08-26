#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QWidget>

namespace Ui {
class fileOperation;
}

class fileOperation : public QWidget
{
    Q_OBJECT

public:
    explicit fileOperation(QWidget *parent = nullptr);
    ~fileOperation();

signals:
    void startConnect(unsigned short,QString);
    void sendFile(QString path);

private slots:
    void on_sendBtn_clicked();

    void on_cancelBtn_clicked();

    void on_selectFileBtn_clicked();

    void on_connectServer_clicked();

private:
    Ui::fileOperation *ui;
};

#endif // FILEOPERATION_H
