#include "randomforest.h"
#include "cosinesim.h"
#include <QtDebug>

randomForest::randomForest()
{

}

bool randomForest::makeDecision(QVector<QImage> allPic,QImage shot)
{
    if(allPic.length() == 0){
        return false;
    }
    cosineSim css;
    double max = 0;
    double min = 1;
    for(int i=0;i<allPic.length();i++)
    {
        for(int j=1;j<5;j++)
        {
            double temp = css.cal(shot,allPic[i],j);
            if (temp>max)
                max = temp;
            if (temp<min)
                min = temp;
        }
    }
    qDebug() << max;
    return max>thresholds;
}
