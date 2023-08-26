#include "register.h"
#include "ui_register.h"
#include "widget.h"
#include "QMessageBox"
#include "uishadow.h"
#include <QRegExpValidator>

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    //restrictions on username and password: contains 1-10 letters or numbers
    QRegExp regex("^[A-Za-z0-9]{1,10}$");
    QRegExpValidator *uservalidator = new QRegExpValidator(regex, ui->regUsernameInput);
    QRegExpValidator *pswvalidator = new QRegExpValidator(regex, ui->regPwdInput);
    ui->regUsernameInput->setValidator(uservalidator);
    ui->regPwdInput->setValidator(pswvalidator);

    //create shadow on widget
    Shadow *shadow = new Shadow;
    shadow->applyToWidget(ui->widget);

}

Register::~Register()
{
delete ui;
}

void Register::on_confirmBtn_clicked()
{
    if(ui->regPwdInput->text() != ui->regRepeatPwdInput->text()){
        QMessageBox::warning(this,tr("Warning"),tr("Differnet passwords"),QMessageBox::Yes);
        ui->regPwdInput->clear();
        ui->regRepeatPwdInput->clear();
    }
    //else if() username repetiton
    else{
        int id = 1001;
        QString stringid = QString(id);
        QString username = ui->regUsernameInput->text();
        QString password = ui->regPwdInput->text();
        //pass data to database

        QMessageBox::information(this,tr("Note"),tr("Register successfully"));
        this->close();
        Widget *login = new Widget;
        login->show();

    }

}

void Register::on_cancelBtn_clicked()
{
    this->close();
    Widget *login = new Widget;
    login->show();

}
