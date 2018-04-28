#include "frstbudextractor.h"
#include <opencv2/imgproc.hpp>
FrstBudExtractor::FrstBudExtractor()
{
  gaussianSize = 1;
  frst.setRanges({5,6,7,8,9,10,11,12});
  frst.setGaussianStdDev(5);
  frst.setRadialStrictness(2);
  frst.setGradientThreshold(0.1);
}

std::vector<Circle> FrstBudExtractor::extractBuds(const cv::Mat &image, const cv::Mat &mask)
{
  cv::Mat blurredImage;
  cv::GaussianBlur(image,blurredImage,cv::Size(9,9),5);
  return frst.getCircles(blurredImage,mask);
}

float FrstBudExtractor::getRadialStrictness() const
{
  return frst.getRadialStrictness();
}

void FrstBudExtractor::setRadialStrictness(float value)
{
  frst.setRadialStrictness(value);
}

int FrstBudExtractor::getRangeMin() const
{
  std::vector<int> ranges = getRanges();
  if(ranges.size()==0)
    return 0;
  int value = ranges[0];
  for(size_t i=1;i<ranges.size();i++)
  {
    if(ranges[i]<value)
      value = ranges[i];
  }
  return value;
}

int FrstBudExtractor::getRangeMax() const
{
  std::vector<int> ranges = getRanges();
  if(ranges.size()==0)
    return 0;
  int value = ranges[0];
  for(size_t i=1;i<ranges.size();i++)
  {
    if(ranges[i]>value)
      value = ranges[i];
  }
  return value;
}

std::vector<int> FrstBudExtractor::getRanges() const
{
  return frst.getRanges();
}

void FrstBudExtractor::setRanges(const std::vector<int> &value)
{
  frst.setRanges(value);
}

void FrstBudExtractor::setRanges(int min, int max)
{
  std::vector<int> ranges;
  for(int i=min; i<=max ;i++)
  {
    ranges.push_back(i);
  }
  setRanges(ranges);
}

float FrstBudExtractor::getGradientThreshold() const
{
return frst.getGradientThreshold();
}

void FrstBudExtractor::setGradientThreshold(float value)
{
  frst.setGradientThreshold(value);
}

float FrstBudExtractor::getCircleSuppressionFactor() const
{
  return frst.getCircleSuppressionFactor();
}

void FrstBudExtractor::setCircleSuppressionFactor(float value)
{
  frst.setCircleSuppressionFactor(value);
}

float FrstBudExtractor::getGaussianPostStdDev() const
{
  return frst.getGaussianStdDev();
}

void FrstBudExtractor::setGaussianPostStdDev(float value)
{
  frst.setGaussianStdDev(value);
}

int FrstBudExtractor::getGaussianPreSize() const
{
  return gaussianSize;
}

void FrstBudExtractor::setGaussianPreSize(int value)
{
  gaussianSize = value;
}
