#include "fileoperation.h"
#include "ui_fileoperation.h"
#include "QFileDialog"

fileOperation::fileOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileOperation)
{
    ui->setupUi(this);

}

fileOperation::~fileOperation()
{
    delete ui;
}

void fileOperation::on_sendBtn_clicked()
{
    if(ui->progressBar->value()!=100){

    }
    else{
        //send
    }
}

void fileOperation::on_cancelBtn_clicked()
{
    this->close();
}

void fileOperation::on_selectFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*);;Text Files (*.txt)"));
    ui->fileNameInput->insert(fileName);
}
