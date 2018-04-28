#ifndef FRSTBUDEXTRACTOR_H
#define FRSTBUDEXTRACTOR_H

#include "budextractor.h"
#include "fastradialsymmetrytransform.h"

class FrstBudExtractor : public BudExtractor
{
  FastRadialSymmetryTransform frst;
  int gaussianSize;
public:
  FrstBudExtractor();
  virtual std::vector<Circle> extractBuds(const cv::Mat& image, const cv::Mat& mask);
  float getRadialStrictness() const;
  void setRadialStrictness(float value);
  int getRangeMin() const;
  int getRangeMax() const;
  std::vector<int> getRanges() const;
  void setRanges(const std::vector<int> &value);
  void setRanges(int min, int max);
  float getGradientThreshold() const;
  void setGradientThreshold(float value);
  float getCircleSuppressionFactor() const;
  void setCircleSuppressionFactor(float value);
  float getGaussianPostStdDev() const;
  void setGaussianPostStdDev(float value);
  int getGaussianPreSize() const;
  void setGaussianPreSize(int value);
};

#endif // FRSTBUDEXTRACTOR_H
