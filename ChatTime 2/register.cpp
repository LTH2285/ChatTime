/*
 * @Author: LTH
 * @Date: 2023-08-27 22:39:44
 * @LastEditTime: 2023-08-28 09:51:59
 * @FilePath: \Mini-term-assignments-for-junior-year\ChatTime 2\register.cpp
 * @Description:
 * Copyright (c) 2023 by LTH, All Rights Reserved.
 */
#include "register.h"
#include "ui_register.h"
#include "widget.h"
#include "QMessageBox"
#include "uishadow.h"
#include <QMessageBox>
#include <QRegExpValidator>

Register::Register(QWidget *parent) : QWidget(parent),
                                      ui(new Ui::Register)
{
    // 初始化ui
    ui->setupUi(this);
    // 创建正则表达式验证器
    QRegExp regex("^[A-Za-z0-9]{1,10}$");
    QRegExpValidator *uservalidator = new QRegExpValidator(regex, ui->regUsernameInput);
    QRegExpValidator *pswvalidator = new QRegExpValidator(regex, ui->regPwdInput);
    // 设置验证器
    ui->regUsernameInput->setValidator(uservalidator);
    ui->regPwdInput->setValidator(pswvalidator);

    // 创建阴影
    Shadow *shadow = new Shadow;
    // 将阴影应用到窗口上
    shadow->applyToWidget(ui->widget);
}

Register::~Register()
{
    // 释放ui
    delete ui;
}

void Register::on_confirmBtn_clicked()
{
    // 检查两次密码输入是否相同
    if (ui->regPwdInput->text() != ui->regRepeatPwdInput->text())
    {
        // 如果不同，弹出警告框，提示两次密码不同
        QMessageBox::warning(this, tr("Warning"), tr("Differnet passwords"), QMessageBox::Yes);
        // 清空密码输入框
        ui->regPwdInput->clear();
        ui->regRepeatPwdInput->clear();
    }
    else
    {
        // 如果相同，弹出注册成功框
        QString username = ui->regUsernameInput->text();
        QString password = ui->regPwdInput->text();
        QMessageBox::information(this, tr("Note"), tr("Register successfully"));
        // 关闭窗口
        this->close();
        Widget *login = new Widget;
        login->show();
    }
}

void Register::on_cancelBtn_clicked()
{
    // 关闭窗口
    this->close();
    Widget *login = new Widget;
    login->show();
}
