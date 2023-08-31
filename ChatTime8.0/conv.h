#ifndef CONV_H
#define CONV_H

#include <QImage>
#include <QPixmap>
#include <QVector>


class conv
{
public:
    conv();
    QVector<int> imageToVector(const QImage &image);
    QVector<int> edgeConvolution(QVector<int> input);
    QVector<int> sharpenConvolution(QVector<int> input);
    QImage vecToImage(QVector<int> vec, int width, int height);

    QVector<QImage> findAllUserImages(int userID);

private:

};

#endif // CONV_H
