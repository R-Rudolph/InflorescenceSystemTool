#ifndef FASTRADIALSYMMETRYTRANSFORM_H
#define FASTRADIALSYMMETRYTRANSFORM_H

/*
 * Implementation of Fast Radial Symmetry Transform
 * as presentend in
 * A Fast Radial Symmetry Transform for Detecting Points of Interest (Gareth Loy and Alexander Zelinsky)
 * Implementation by:
 * Robert Rudolph
 */

#include <opencv2/core.hpp>
#include <vector>
#include "circle.h"

class FastRadialSymmetryTransform
{
  std::vector<int> ranges; //values of n for which frst is performed
  float radialStrictness; //alpha
  float gaussianStdDev; //for gaussian A, standard deviation is scaled linearly with range
  float magnitudeThreshold; //magnitude threshold beta. value between 0 and 1
  float circleSuppressionFactor; //factor to which detected circles around a previously selected circle will be suppressed
  cv::Mat transform(const cv::Mat &gradient, int n, const cv::Mat& mask=cv::Mat()) const;
  inline bool pointIsOk(const cv::Point& p, const cv::Mat& image) const;
  void nonMaximaSuppression(const cv::Mat& image, cv::Mat& maxMap) const;
  int findRadius(const std::vector<cv::Mat>&, const cv::Point& p) const;
  void addToVector(cv::Point point, std::vector<cv::Point> &vector) const;
  std::vector<cv::Point> circlePoints(int radius) const;
public:
  FastRadialSymmetryTransform();
  cv::Mat transform(const cv::Mat& image, const cv::Mat& mask=cv::Mat()) const;
  std::vector<Circle> getCircles(const cv::Mat& image, const cv::Mat& mask=cv::Mat()) const;
  float getRadialStrictness() const;
  void setRadialStrictness(float value);
  std::vector<int> getRanges() const;
  void setRanges(const std::vector<int> &value);
  float getGradientThreshold() const;
  void setGradientThreshold(float value);
  float getCircleSuppressionFactor() const;
  void setCircleSuppressionFactor(float value);
  float getGaussianStdDev() const;
  void setGaussianStdDev(float value);
};

#endif // FASTRADIALSYMMETRYTRANSFORM_H
