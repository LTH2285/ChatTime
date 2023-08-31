#ifndef COSINESIM_H
#define COSINESIM_H

#include <QtMath>
#include <QImage>

class cosineSim
{
public:
    cosineSim();
    double cal(QImage img1,QImage img2,int i);
};

#endif // COSINESIM_H
