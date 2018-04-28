#include "chtbudextractor.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
int ChtBudExtractor::getLcnInputGaussRadius() const
{
  return lcnInputGaussRadius;
}

void ChtBudExtractor::setLcnInputGaussRadius(int value)
{
  lcnInputGaussRadius = value;
}

int ChtBudExtractor::getLcnGaussRadius() const
{
  return lcnGaussRadius;
}

void ChtBudExtractor::setLcnGaussRadius(int value)
{
  lcnGaussRadius = value;
}

int ChtBudExtractor::getLcnOutputGaussRadius() const
{
  return lcnOutputGaussRadius;
}

void ChtBudExtractor::setLcnOutputGaussRadius(int value)
{
  lcnOutputGaussRadius = value;
}

int &ChtBudExtractor::getLcnInputGaussRadiusRef()
{
  return lcnInputGaussRadius;
}

int &ChtBudExtractor::getLcnGaussRadiusRef()
{
  return lcnGaussRadius;
}

int &ChtBudExtractor::getLcnOutputGaussRadiusRef()
{
  return lcnOutputGaussRadius;
}

ChtBudExtractor::ChtBudExtractor()
  : cannyOp(12,24), cht(7,15)
{
  localContrastNormalizationOffset = 128;
  localContrastNormalizationFactor = 64;
  lcnInputGaussRadius = 4;
  lcnGaussRadius = 10;
  lcnOutputGaussRadius = 4;
  cht.setThreshold(0.33);
}
#include <iostream>
std::vector<Circle> ChtBudExtractor::extractBuds(const cv::Mat &image, const cv::Mat &mask)
{
  cv::Mat normalized;
  cv::cvtColor(image,normalized,CV_BGR2GRAY);
  cv::GaussianBlur(normalized,normalized,cv::Size(lcnInputGaussRadius*2+1,lcnInputGaussRadius*2+1),0);
  normalized = localContrastNormalization(normalized,lcnGaussRadius,localContrastNormalizationFactor,localContrastNormalizationOffset);
  cv::GaussianBlur(normalized,normalized,cv::Size(lcnOutputGaussRadius*2+1,lcnOutputGaussRadius*2+1),0);
  cv::imwrite("/home/underscore/Desktop/test.png",normalized);
  normalized.convertTo(normalized,CV_8U);
  cannyOp.processImage(normalized,mask);
  std::vector<Circle> result = cht.extractCircles(cannyOp);
  return result;
}

void ChtBudExtractor::setUpperThreshold(int value)
{
  cannyOp.setThresholdHigh(value);
}

int ChtBudExtractor::getUpperThreshold() const
{
  return cannyOp.getThresholdHigh();
}

int &ChtBudExtractor::getUpperThresholdRef()
{
  return cannyOp.getThresholdHighRef();
}

void ChtBudExtractor::setLowerThreshold(int value)
{
  cannyOp.setThresholdLow(value);
}

int ChtBudExtractor::getLowerThreshold() const
{
  return cannyOp.getThresholdLow();
}

int &ChtBudExtractor::getLowerThresholdRef()
{
  return cannyOp.getThresholdLowRef();
}

void ChtBudExtractor::setChtThreshold(float value)
{
  cht.setThreshold(value);
}

void ChtBudExtractor::setAdjacentBlockingFactor(float value)
{
  cht.setAdjacentBlockingFactor(value);
}

float ChtBudExtractor::getChtThreshold() const
{
  return cht.getThreshold();
}

float ChtBudExtractor::getAdjacentBlockingFactor() const
{
  return cht.getAdjacentBlockingFactor();
}

float &ChtBudExtractor::getChtThresholdRef()
{
  return cht.getThresholdRef();
}

float &ChtBudExtractor::getAdjacentBlockingFactorRef()
{
  return cht.getAdjacentBlockingFactorRef();
}

