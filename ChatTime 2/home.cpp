#include "home.h"
#include "ui_home.h"
#include <QListWidget>
#include "QListWidgetItem"
#include "widget.h"
#include "QMessageBox"
#include "chat.h"
#include "aichat.h"
#include "uishadow.h"


QList<QString> contactList = {"fuyu","wangzhihe","luoyang"};


Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    Shadow *sha = new Shadow;
    sha->applyToWidget(ui->friendList);

    Home::CreateFriendList();
    ui->onlineNumber->setText(QString("now %1 users active").arg(contactList.length()));

}

Home::~Home()
{
    delete ui;
}
void Home::StartChat(){
    QListWidgetItem *selectedItem = ui->friendList->currentItem();

    if(selectedItem){
        QString contactName = selectedItem->text();
        this->close();
        Chat *chat = new Chat(contactName);
        chat->show();


    }
    else{
        QMessageBox::warning(this, "Warning", "Please select a contact",QMessageBox::Yes);
    }
}
void Home::CreateFriendList(){
    //link server
    ui->friendList->addItems(contactList);
}

void Home::on_logoutBtn_clicked()
{
    this->close();
    Widget *login = new Widget;
    login->show();
}

void Home::on_startChatBtn_clicked()
{
    StartChat();
}


void Home::on_AiChatStartBtn_clicked()
{
    this->close();
    Aichat *w = new Aichat();
    w->show();

}

void Home::on_friendList_itemDoubleClicked(QListWidgetItem *item)
{
    StartChat();
}
