/*
 * @Author: LTH
 * @Date: 2023-08-27 22:39:44
 * @LastEditTime: 2023-08-28 09:20:29
 * @FilePath: \Mini-term-assignments-for-junior-year\ChatTime 2\fileoperation.cpp
 * @Description:
 * Copyright (c) 2023 by LTH, All Rights Reserved.
 */
#include "fileoperation.h"
#include "ui_fileoperation.h"
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include "sendfile.h"
fileOperation::fileOperation(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::fileOperation)
{
    // 初始化窗口
    ui->setupUi(this);
    // 设置服务器IP地址
    ui->ip->setText("127.0.0.1");
    // 设置服务器端口号
    ui->port->setText("8989");
    // 设置进度条的范围
    ui->progressBar->setRange(0, 100);
    // 设置进度条的当前值
    ui->progressBar->setValue(0);

    // 创建线程
    QThread *t = new QThread;
    // 创建发送文件类
    SendFile *worker = new SendFile;

    // 移动线程到线程管理器
    worker->moveToThread(t);

    // 将发送文件的槽函数挂载到窗口的发送文件信号
    connect(this, &fileOperation::sendFile, worker, &SendFile::sendFile);
    // 将连接服务器的槽函数挂载到窗口的连接服务器信号
    connect(this, &fileOperation::startConnect, worker, &SendFile::connectServer);

    // 将连接服务器成功的槽函数挂载到窗口的连接服务器成功信号
    connect(worker, &SendFile::connectedOK, this, [=]()
            { QMessageBox::information(this, "连接服务器", "已经成功连接了服务器，恭喜！"); });

    // 将游戏结束的槽函数挂载到窗口的游戏结束信号
    connect(worker, &SendFile::gameover, this, [=]()
            {
        t->quit();
        t->wait();
        worker->deleteLater();
        t->deleteLater(); });
    connect(worker, &SendFile::curPercent, ui->progressBar, &QProgressBar::setValue);
    // 启动线程
    t->start();
}

fileOperation::~fileOperation()
{
    // 销毁窗口
    delete ui;
}

void fileOperation::on_selectFileBtn_clicked()
{
    // 打开文件对话框，获取选择的文件路径
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*);;Text Files (*.txt)"));
    // 如果文件路径为空，则弹出警告框
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, "打开文件", "选择的文件路径不能为空!");
        return;
    }
    // 将文件路径添加到文本框中
    ui->fileNameInput->insert(fileName);
}

void fileOperation::on_sendBtn_clicked()
{
    // 如果进度条不为100，则不进行发送
    if (ui->progressBar->value() != 100)
    {
    }
    // 否则，发送
    else
    {
        // send
    }
    // 发送文件
    emit sendFile(ui->fileNameInput->text());
}

void fileOperation::on_cancelBtn_clicked()
{
    // 关闭窗口
    this->close();
}

void fileOperation::on_connectServer_clicked()
{
    // 获取IP地址和端口号
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    // 开始连接服务器
    emit startConnect(port, ip);
}