#ifndef TOGRAY_H
#define TOGRAY_H

#include <QImage>
#include <QVector>

class toGray
{
public:
    toGray();
    QImage toGrayPicture(QImage image);
    QVector<int> toGrayArray(QImage image,int i=1);
};

#endif // TOGRAY_H
