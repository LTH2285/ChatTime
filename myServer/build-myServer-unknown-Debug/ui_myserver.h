/********************************************************************************
** Form generated from reading UI file 'myserver.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYSERVER_H
#define UI_MYSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_myServer
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QListWidget *listWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *myServer)
    {
        if (myServer->objectName().isEmpty())
            myServer->setObjectName(QString::fromUtf8("myServer"));
        myServer->resize(800, 600);
        centralwidget = new QWidget(myServer);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(90, 30, 121, 31));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(90, 80, 441, 291));
        myServer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(myServer);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 28));
        myServer->setMenuBar(menubar);
        statusbar = new QStatusBar(myServer);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        myServer->setStatusBar(statusbar);

        retranslateUi(myServer);

        QMetaObject::connectSlotsByName(myServer);
    } // setupUi

    void retranslateUi(QMainWindow *myServer)
    {
        myServer->setWindowTitle(QApplication::translate("myServer", "myServer", nullptr));
        label->setText(QApplication::translate("myServer", "\345\275\223\345\211\215\345\234\250\347\272\277\344\272\272\346\225\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class myServer: public Ui_myServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYSERVER_H
