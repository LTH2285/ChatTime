#include "conv.h"

conv::conv()
{

}

QVector<int> conv::imageToVector(const QImage &image)
{
    QVector<int> vector(image.width() * image.height());

      for (int y = 0; y < image.height(); ++y) {
        const uchar *sl = image.scanLine(y);
        memcpy(&vector[y * image.width()], sl, image.bytesPerLine());
      }

      return vector;
}

QImage conv::vecToImage(QVector<int> vec, int width, int height) {
    uchar* data = new uchar[vec.size()];
      for(int i = 0; i < vec.size(); ++i) {
        data[i] = (uchar)vec[i];
      }

      QImage image(data, width, height, QImage::Format_Grayscale8);

      // 将int类型数据归一化到0-255范围
      int max = *std::max_element(vec.begin(), vec.end());
      int min = *std::min_element(vec.begin(), vec.end());

      for (int i = 0; i < vec.size(); ++i) {
          image.bits()[i] = (vec[i] - min) * 255 / (max - min);
      }

      return image;
}


QVector<int> edgeConvolution(QVector<int> input) {
  QVector<int> output(input.size());
  int  kernel[3][3] = {
      {-1, -1, -1},
      {-1, 8, -1},
      {-1, -1, -1}
    };

  int kernelRadius = 1;

  for (int i = kernelRadius; i < input.size() - kernelRadius; ++i) {
    int sum = 0;
    for (int j = -kernelRadius; j <= kernelRadius; ++j) {
      for (int k = -kernelRadius; k <= kernelRadius; ++k) {
        sum += input[i + j] * kernel[j + kernelRadius][k + kernelRadius];
      }
    }
    output[i] = sum;
  }

  return output;
}

QVector<int> sharpenConvolution(QVector<int> input) {
  QVector<int> output(input.size());
  int kernel[3][3] = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0}
  };
  int kernelRadius = 1;

  for (int i = kernelRadius; i < input.size() - kernelRadius; ++i) {
    int sum = 0;
    for (int j = -kernelRadius; j <= kernelRadius; ++j) {
      for (int k = -kernelRadius; k <= kernelRadius; ++k) {
        sum += input[i + j] * kernel[j + kernelRadius][k + kernelRadius];
      }
    }
    output[i] = sum;
  }

  return output;
}

QVector<QImage> findAllUserImages(int userID)
{

}

