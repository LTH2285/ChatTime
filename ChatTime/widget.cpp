#include "widget.h"
#include "ui_widget.h"
#include "register.h"
#include "QMessageBox"
#include "home.h"
#include "uishadow.h"
#include "register.h"



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
    if(ui->idInput->text().trimmed() == tr("1001") && ui->passwordInput->text() == tr("123")){
        QMessageBox::information(NULL, tr("Note"), tr("Login successfully!"));
        this->close();
        Home *home = new Home();
        home->show();

    }
    else{
        QMessageBox::warning(this, tr("Warning"), tr("Id and password do not match!"),QMessageBox::Yes);
        ui->idInput->clear();
        ui->passwordInput->clear();
        ui->idInput->setFocus();
    }
}

void Widget::on_regosterBtn_clicked()
{
    this->close();
    Register *reg = new Register();
    reg->show();
}

void Widget::on_idInput_editingFinished()
{

}

void Widget::on_passwordInput_editingFinished()
{

}

