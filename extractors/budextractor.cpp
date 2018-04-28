#include "budextractor.h"
#include <opencv2/imgproc.hpp>

cv::Mat BudExtractor::localContrastNormalization(const cv::Mat &input, int radius, double normalizationFactor, double offset) const
{
  int size = 2*radius+1;
  cv::Mat inputFloat;
  input.convertTo(inputFloat,CV_32F);
  cv::Mat diff, result, stdDev;
  cv::GaussianBlur(inputFloat,diff,cv::Size(size,size),0);
  diff = inputFloat - diff;
  cv::pow(diff,2,stdDev);
  cv::GaussianBlur(stdDev,stdDev,cv::Size(size,size),0);
  cv::sqrt(stdDev,stdDev);
  double mean = cv::mean(stdDev)[0];
  cv::max(stdDev,mean,stdDev);
  cv::divide(diff,stdDev,result,normalizationFactor);
  return result + offset;
}

size_t BudExtractor::extractBudsNum(const cv::Mat &image, cv::Mat &mask)
{
  return extractBuds(image,mask).size();
}

cv::Mat BudExtractor::budImage(const cv::Mat &image, cv::Mat &mask)
{
  std::vector<Circle> buds = extractBuds(image,mask);
  cv::Mat result = image;
  for(size_t i=0;i<buds.size();i++)
  {
    cv::circle(result,buds[i].center,buds[i].radius,cv::Scalar(255,0,0));
  }
  return result;
}
