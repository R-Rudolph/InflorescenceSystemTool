#ifndef CIRCLE_H
#define CIRCLE_H

#include <opencv2/core/core.hpp>

struct Circle
{
  cv::Point center;
  int radius;
  float certainty;
  Circle(cv::Point center, int radius, float certainty=0.0);
  bool operator <(const Circle& other);
};

#endif // CIRCLE_H
