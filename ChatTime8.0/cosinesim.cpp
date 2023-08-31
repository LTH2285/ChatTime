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

    for (int i = 0; i < len; i++)
    {
        dot += a1[i] * a2[i];
        denom_a += a1[i] * a1[i];
        denom_b += a2[i] * a2[i];
    }

    return dot / (sqrt(denom_a) * sqrt(denom_b));
}
