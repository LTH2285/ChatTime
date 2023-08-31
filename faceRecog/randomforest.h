#ifndef RANDOMFOREST_H
#define RANDOMFOREST_H
#include "cosinesim.h"

class randomForest
{
public:
    randomForest();
    bool makeDecision(QVector<QImage> allPic,QImage shot);
    double thresholds = 0.2;
};

#endif // RANDOMFOREST_H
