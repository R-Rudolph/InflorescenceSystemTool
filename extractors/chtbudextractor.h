#ifndef CHTBUDEXTRACTOR_H
#define CHTBUDEXTRACTOR_H

#include "budextractor.h"
#include "cannyoperator.h"
#include "CircularHoughTransform.h"

class ChtBudExtractor : public BudExtractor
{
  double localContrastNormalizationOffset;
  double localContrastNormalizationFactor;
  int lcnInputGaussRadius;
  int lcnGaussRadius;
  int lcnOutputGaussRadius;
  CannyOperator cannyOp;
  CircularHoughTransform cht;
public:
  ChtBudExtractor();
  virtual std::vector<Circle> extractBuds(const cv::Mat& image, const cv::Mat& mask);
  void setUpperThreshold(int value);
  int getUpperThreshold() const;
  int& getUpperThresholdRef();
  void setLowerThreshold(int value);
  int getLowerThreshold() const;
  int& getLowerThresholdRef();
  void setChtThreshold(float value);
  void setAdjacentBlockingFactor(float value);
  float getChtThreshold() const;
  float getAdjacentBlockingFactor() const;
  float& getChtThresholdRef();
  float& getAdjacentBlockingFactorRef();
  int getLcnInputGaussRadius() const;
  void setLcnInputGaussRadius(int value);
  int getLcnGaussRadius() const;
  void setLcnGaussRadius(int value);
  int getLcnOutputGaussRadius() const;
  void setLcnOutputGaussRadius(int value);

  int& getLcnInputGaussRadiusRef();
  int& getLcnGaussRadiusRef();
  int& getLcnOutputGaussRadiusRef();
};

#endif // CHTBUDEXTRACTOR_H
