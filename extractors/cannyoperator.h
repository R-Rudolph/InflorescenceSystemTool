#ifndef CANNYOPERATOR_H
#define CANNYOPERATOR_H

#include <opencv2/core.hpp>
#include <list>

class CannyOperator
{
  enum EdgeStatus
  {
    EDGE_NONE = 0,
    EDGE_WEAK = 1,
    EDGE_STRONG = 255
  };
  int thresholdLow, thresholdHigh;
  cv::Mat gradientDirection, gradientMagnitude, thinnedMagnitude, mask;
  inline bool greaterMagnitude(int row1, int col1, int row2, int col2);
  inline bool greaterEqualMagnitude(int row1, int col1, int row2, int col2);
  inline void nonMaxSuppression(int row, int col, std::list<cv::Point>& weakEdges);
  inline void expandList(const cv::Point& point, std::vector<cv::Point>& list, bool& foundStrong);
  inline void trackEdge(const cv::Point& edgePoint);
  int directionValue(int row, int col);
  //cv::Point add(const cv::Point& point,int )
public:
  CannyOperator(int thresholdLow, int thresholdHigh);
  void processImage(const cv::Mat& image, const cv::Mat& mask);
  void processImage(const cv::Mat& image);
  const cv::Mat& getEdges() const;
  const cv::Mat& getGradientDirection() const;
  int getThresholdLow() const;
  int& getThresholdLowRef();
  void setThresholdLow(int value);
  int getThresholdHigh() const;
  int& getThresholdHighRef();
  void setThresholdHigh(int value);
  const cv::Mat& getMask() const;
};

#endif // CANNYOPERATOR_H
