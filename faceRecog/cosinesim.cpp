#include "cosinesim.h"
#include "togray.h"

cosineSim::cosineSim()
{

}

double cosineSim::cal(QImage img1,QImage img2,int i)
{
    toGray tog;
    QVector<int> a1,a2;
    a1 = tog.toGrayArray(img1,i);
    a2 = tog.toGrayArray(img2,i);
    int len = 88*88;

    double dot = 0.0;
    double denom_a = 0.0;
    double denom_b = 0.0;

    for (int j = 0; j< len; j++)
    {
        dot += a1[j] * a2[j];
        denom_a += a1[j] * a1[j];
        denom_b += a2[j] * a2[j];
    }

    if(dot == 0 || denom_a == 0 || denom_b == 0)
    {
        return -1;
    }

    return dot / (sqrt(denom_a) * sqrt(denom_b));
}
