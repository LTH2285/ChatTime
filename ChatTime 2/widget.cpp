#include "widget.h"
#include "ui_widget.h"
#include "register.h"
#include "QMessageBox"
#include "home.h"
#include "uishadow.h"



// login

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    Shadow *shadow = new Shadow;
    shadow->applyToWidget(ui->widget);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_loginBtn_clicked()
{
    //data to server check in database
    if(ui->usernameInput->text().trimmed() == tr("test") && ui->passwordInput->text() == tr("123")){
        QMessageBox::information(NULL, tr("Note"), tr("Login successfully!"));
        this->close();
        Home *home = new Home();
        home->show();

    }
    else{
        QMessageBox::warning(this, tr("Warning"), tr("Username and password do not match!"),QMessageBox::Yes);
        ui->usernameInput->clear();
        ui->passwordInput->clear();
        ui->usernameInput->setFocus();
    }
}

void Widget::on_regosterBtn_clicked()
{
    this->close();
    Register *reg = new Register();
    reg->show();
}

void Widget::on_usernameInput_editingFinished()
{

}

void Widget::on_passwordInput_editingFinished()
{

}
