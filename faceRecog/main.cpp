#include "MainWindow.h"
#include "conv.h"
#include "togray.h"
#include "cosinesim.h"
#include "randomforest.h"
#include "addpic.h"

#include <QApplication>
#include <QImage>
#include <QtDebug>
#include <QMetaType>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    addPic ap;
    ap.show();
//    MainWindow w;
//    w.show();


//    QImage *image = new QImage("/home/liutinghan_1120210576/Pictures/login.jpg");
//    toGray tg;
//    QImage gray_image = tg.toGrayPicture(*image);
//    gray_image.save("/home/liutinghan_1120210576/Pictures/lth_gray_conv.jpg");

//    int* array;
//    qDebug() << tg.toGrayArray(*image,array);
//    free(array)

//    free(image);

//    QImage *image1 = new QImage("/home/liutinghan_1120210576/Desktop/picture/lth1.jpg");
//    QImage *image2 = new QImage("/home/liutinghan_1120210576/Desktop/picture/wzh3.jpg");
//    cosineSim css;
//    for(int i = 1;i<5;i++)
//    {
//        qDebug() << css.cal(*image1,*image2,i);
//    }
//    free(image1);
//    free(image2);

//    QImage *image1 = new QImage("/home/liutinghan_1120210576/Pictures/login.jpg");
//    QImage *image2 = new QImage("/home/liutinghan_1120210576/Pictures/login1.jpg");
//    cosineSim css;
//    qDebug() << css.cal(*image1,*image2,4);
//    free(image1);
//    free(image2);



//    根据阈值进行判断

//    randomForest rf;
//    int ID;//需要连接
//    QImage *shot = new QImage("login.jpg");

//    QVector<QImage> allPic;
//    QVector<QImage> allPic = findAllUserImages(ID);//需要连接
//    rf.makeDecision(allPic,*shot);

//    QString path = "/home/liutinghan_1120210576/Desktop/picture/txs3.jpg";
//    QImage *image = new QImage(path);
//    if(image->width()>800)
//    {
//        QImage out = image->scaled(800,450,Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        bool flag = out.save(path, "JPG", 15);
//            qDebug() << flag;
//    }

    return a.exec();
}
