/*
 * @Author: LTH
 * @Date: 2023-08-27 22:39:44
 * @LastEditTime: 2023-08-28 09:47:02
 * @FilePath: \Mini-term-assignments-for-junior-year\ChatTime 2\facecharacter.cpp
 * @Description:
 * Copyright (c) 2023 by LTH, All Rights Reserved.
 */
#include "facecharacter.h"
#include "ui_facecharacter.h"
#include "QListWidgetItem"

// 定义一个QList<QString>类型的变量emojilist，用于存放表情符号
QList<QString> emojilist = {"😀",
                            "😃",
                            "😄",
                            "😁",
                            "😆",
                            "😅",
                            "😂",
                            "🤣",
                            "😊",
                            "😇"
                            "\(＾◡＾)/",
                            "(╯°□°）╯︵ ┻━┻",
                            "¯\_(ツ)_/¯",
                            "(づ｡◕‿‿◕｡)づ",
                            "(｡♥‿♥｡)",
                            "(ಠ_ಠ)",
                            "(•ω•)",
                            "(●'◡'●)",
                            "(⌐■_■)",
                            "(╥_╥)"};

// 定义一个FaceCharacter类的构造函数，接收一个QWidget*类型的参数parent
FaceCharacter::FaceCharacter(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::FaceCharacter)
{
    // 将表情符号添加到faceCharacter控件中
    ui->setupUi(this);
    // 将表情符号添加到faceCharacter控件中，并将表情符号设置为选中状态
    ui->faceCharacter->addItems(emojilist);
}

// 定义一个FaceCharacter类的析构函数
FaceCharacter::~FaceCharacter()
{
    // 释放ui控件
    delete ui;
}

// 定义一个emojiSelected函数，接收一个QString类型的参数emoji
void FaceCharacter::on_insertBtn_clicked()
{
    // 调用emojiSelected函数，传入表情符号emoji
    emit emojiSelected(ui->faceCharacter->currentItem()->text());
    // 关闭当前窗口
    this->close();
}

// 定义一个on_cancelBtn_clicked函数，关闭当前窗口
void FaceCharacter::on_cancelBtn_clicked()
{
    // 调用close函数，关闭当前窗口
    this->close();
}