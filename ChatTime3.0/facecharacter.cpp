#include "facecharacter.h"
#include "ui_facecharacter.h"
#include "QListWidgetItem"

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

FaceCharacter::FaceCharacter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceCharacter)
{
    ui->setupUi(this);
    ui->faceCharacter->addItems(emojilist);
}

FaceCharacter::~FaceCharacter()
{
    delete ui;
}

void FaceCharacter::on_insertBtn_clicked()
{
    emit emojiSelected(ui->faceCharacter->currentItem()->text());
    this->close();
}

void FaceCharacter::on_cancelBtn_clicked()
{
    this->close();
}
