#include "togray.h"
#include <QDebug>

toGray::toGray()
{

}

QImage toGray::toGrayPicture(QImage image)
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    default:
        break;
    }

    int size = 450;
    int x = (width - size) / 2;
    int y = (height - size) / 2;
    ret = ret.copy(x, y, size, size);

//    qDebug() << ret.height() <<ret.width();

    uchar *bits = ret.bits();
    int bytesPerLine = ret.bytesPerLine();
    int imageSize = bytesPerLine * size;
//    qDebug() << bytesPerLine << imageSize;
    uchar *data = new uchar[452*450];

    memcpy(data, bits, imageSize);

//    uchar **data2d = new uchar*[450];
//    for (int i = 0; i < 450; i++) {
//      data2d[i] = &data[i * 450];
//    }
//    for(int i = 0;i<20;i++)
//    {
//        qDebug() << data2d[100][i];
//    }



    int  kernel[3][3] = {
        {-3, 0, 3},
        {-10, 0, 10},
        {-3, 0, 3}
      };

//    int  kernel[3][3] = {
//        {-3, -10, -3},
//        {0, 0, 0},
//        {3, 10, 3}
//      };

//    int  kernel[3][3] = {
//        {-10, -3, 0},
//        {-3, 0, 3},
//        {0, 3, 10}
//      };

//    int  kernel[3][3] = {
//        {10, 3, 0},
//        {3, 0, -3},
//        {0, -3, -10}
//      };

//    int  kernel[3][3] = {
//        {-1, -1, -1},
//        {-1, 8, -1},
//        {-1, -1, -1}
//      };





//    int kernel[5][5] = {
//        {0,0,-1,0,0},
//        {0,-1,-2,-1,0},
//        {-1,-2,16,-2,-1},
//        {0,-1,-2,-1,0},
//        {0,0,-1,0,0}
//    };

    int kernelRadius = 1;

    uchar output[size-2*kernelRadius][size-2*kernelRadius];

    for(int i = kernelRadius; i < size - kernelRadius; i++)
    {
        for (int j = kernelRadius; j < size - kernelRadius; j++)
        {
            int temp = 0;
            for (int k = -kernelRadius; k <=kernelRadius; k++)
            {
                for (int l = -kernelRadius; l <=kernelRadius; l++)
                {
                    temp += kernel[kernelRadius+k][kernelRadius+l] * data[450*(i+k)+j+l];
                }
            }
            if (temp > 255)
            {
                temp = 255;
            }
            output[i-kernelRadius][j-kernelRadius] = (uchar)temp;
        }
    }

    QImage image_conv(450, 450, QImage::Format_Grayscale8);
    uchar *bits_conv = image_conv.bits();
    for(int i=0; i<450; i++) {
      memcpy(bits_conv + i * 450, output[i], 450);
    }

    delete[] data;
    return image_conv;
}



QVector<int> toGray::toGrayArray(QImage image,int i)
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    default:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
    }

    int size = image.height();
    int x = (width - size) / 2;
    int y = (height - size) / 2;
    ret = ret.copy(x, y, size, size);

    uchar *bits = ret.bits();
    int bytesPerLine = ret.bytesPerLine();
    int imageSize = bytesPerLine * size;
    uchar *data = new uchar[452*450];

    memcpy(data, bits, imageSize);

    int  kernel[3][3];

    switch (i) {
    case 1:
         kernel[0][0] = -3;
         kernel[0][1] = 0;
         kernel[0][2] = 3;
         kernel[1][0] = -10;
         kernel[1][1] = 0;
         kernel[1][2] = 10;
         kernel[2][0] = -3;
         kernel[2][1] = 0;
         kernel[2][2] = 3;
        break;
    case 2:
        kernel[0][0] = -3;
        kernel[0][1] = -10;
        kernel[0][2] = -3;
        kernel[1][0] = 0;
        kernel[1][1] = 0;
        kernel[1][2] = 0;
        kernel[2][0] = 3;
        kernel[2][1] = 10;
        kernel[2][2] = 3;
       break;
    case 3:
        kernel[0][0] = -10;
        kernel[0][1] = -3;
        kernel[0][2] = -0;
        kernel[1][0] = -3;
        kernel[1][1] = 0;
        kernel[1][2] = 3;
        kernel[2][0] = 0;
        kernel[2][1] = 3;
        kernel[2][2] = 10;
       break;
    case 4:
        kernel[0][0] = 0;
        kernel[0][1] = -3;
        kernel[0][2] = -10;
        kernel[1][0] = 3;
        kernel[1][1] = 0;
        kernel[1][2] = -3;
        kernel[2][0] = 10;
        kernel[2][1] = 3;
        kernel[2][2] = 0;
       break;
    default:
        kernel[0][0] = -1;
        kernel[0][1] = -1;
        kernel[0][2] = -1;
        kernel[1][0] = -1;
        kernel[1][1] = 9;
        kernel[1][2] = -1;
        kernel[2][0] = -1;
        kernel[2][1] = -1;
        kernel[2][2] = -1;
       break;
    };


//    int kernel[5][5] = {
//        {0,0,-1,0,0},
//        {0,-1,-2,-1,0},
//        {-1,-2,16,-2,-1},
//        {0,-1,-2,-1,0},
//        {0,0,-1,0,0}
//    };

    int kernelRadius = 1;

    int output[size-2*kernelRadius][size-2*kernelRadius];

    for(int i = kernelRadius; i < size - kernelRadius; i++)
    {
        for (int j = kernelRadius; j < size - kernelRadius; j++)
        {
            int temp = 0;
            for (int k = -kernelRadius; k <=kernelRadius; k++)
            {
                for (int l = -kernelRadius; l <=kernelRadius; l++)
                {
                    temp += kernel[kernelRadius+k][kernelRadius+l] * data[450*(i+k)+j+l];
                }
            }
            if (temp > 255)
            {
                temp = 255;
            }
            if (temp < 0)
            {
                temp = 0;
            }
            output[i-kernelRadius][j-kernelRadius] = temp;
        }
    }

    QVector<int> temp_vec;

    for(int i=0;i<size-2*kernelRadius;i++)
    {
        for(int j=0;j<size-2*kernelRadius;j++)
        {
            temp_vec.append(output[i][j]);
        }
    }
    return temp_vec;
}
