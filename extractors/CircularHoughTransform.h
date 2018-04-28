#ifndef HOUGHSPACE_H
#define HOUGHSPACE_H

#include <opencv2/core/core.hpp>
#include <list>

#include "circle.h"
#include "cannyoperator.h"
#include "circledrawer.h"

class CircularHoughTransform
{
  CircleDrawer cd;
  int minRadius, maxRadius;
  float threshold;
  float adjacentBlockingFactor;
  std::vector< Circle > extractCirclesForRadius(int radius, const CannyOperator& image);
  bool isLocalMaxiumum(cv::Mat& houghSpace, int col, int row);
  inline void addIfNotZero(cv::Mat& map, std::vector<cv::Point>& list, const cv::Point& point);
  cv::Point findRegionCenter(cv::Mat& map, const cv::Point& point);
public:
  CircularHoughTransform(int minRadius=8, int maxRadius=15);
  bool isValid();
  float getThreshold() const;
  float& getThresholdRef();
  void setThreshold(float value);
  std::vector<Circle> extractCircles(const CannyOperator& image);
  float getAdjacentBlockingFactor() const;
  float& getAdjacentBlockingFactorRef();
  void setAdjacentBlockingFactor(float value);
};

#endif // HOUGHSPACE_H
