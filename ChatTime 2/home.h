#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();
    void CreateFriendList();
    void StartChat();

private slots:
    void on_logoutBtn_clicked();


    void on_startChatBtn_clicked();


    void on_AiChatStartBtn_clicked();

    void on_friendList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::Home *ui;
};

#endif // HOME_H
