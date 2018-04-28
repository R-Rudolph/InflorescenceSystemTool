#ifndef CIRCLEDRAWER_H
#define CIRCLEDRAWER_H

#include<opencv2/core.hpp>
#include<vector>
#include "circle.h"

class CircleDrawer
{
  float partialPortionRad;
  int minRadius, maxRadius;
  std::vector< std::vector<cv::Point> > drawPoints;
  std::vector<cv::Point> circlePoints(int radius);
  void addToVector(cv::Point point, std::vector<cv::Point>& vector);
public:
  CircleDrawer(int minRadius, int maxRadius);
  void draw(Circle circle, cv::Mat& image);
  void drawPartial(Circle circle, float directionRad, cv::Mat& image);
  size_t numPoints(int radius);
};

#endif // CIRCLEDRAWER_H
