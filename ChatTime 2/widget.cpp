#include "widget.h"
#include "ui_widget.h"
#include "register.h"
#include "QMessageBox"
#include "home.h"
#include "uishadow.h"

// 定义Widget类的构造函数，构造窗口
Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    // 将ui指针传入Shadow类
    ui->setupUi(this);
    // 将Shadow类的applyToWidget函数传入ui指针
    Shadow *shadow = new Shadow;
    shadow->applyToWidget(ui->widget);
}

// 定义Widget类的析构函数，析构窗口
Widget::~Widget()
{
    // 删除ui指针
    delete ui;
}

// 点击登录按钮时触发的函数
void Widget::on_loginBtn_clicked()
{
    // 判断用户名和密码是否为空
    if (ui->usernameInput->text().trimmed() == tr("test") && ui->passwordInput->text() == tr("123"))
    {
        // 如果为空，提示用户名和密码不能为空
        QMessageBox::information(NULL, tr("Note"), tr("Login successfully!"));
        // 关闭窗口
        this->close();
        // 创建Home类的对象，显示窗口
        Home *home = new Home();
        home->show();

        void Widget::on_loginBtn_clicked()
    }
    else
    {
        // 如果不为空，提示用户名和密码不匹配
        QMessageBox::warning(this, tr("Warning"), tr("Username and password do not match!"), QMessageBox::Yes);
        // 清空用户名和密码
        ui->usernameInput->clear();
        ui->passwordInput->clear();
        // 获取焦点
        ui->usernameInput->setFocus();
    }
}

// 点击注册按钮时触发的函数
void Widget::on_regosterBtn_clicked()
{
    if (ui->usernameInput->text().trimmed() == tr("test") && ui->passwordInput->text() == tr("123"))
    {
        QMessageBox::information(NULL, tr("Note"), tr("Login successfully!"));
        // 关闭窗口
        this->close();
        Home *home = new Home();
        home->show();
        // 创建注册类的对象，显示窗口
        Register *reg = new Register();
        reg->show();
    }

    // 用户名输入框被编辑完成时触发的函数
    void Widget::on_usernameInput_editingFinished()
    {
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Username and password do not match!"), QMessageBox::Yes);
        ui->usernameInput->clear();
        ui->passwordInput->clear();
        ui->usernameInput->setFocus();

        // 密码输入框被编辑完成时触发的函数
        void Widget::on_passwordInput_editingFinished()
        {
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