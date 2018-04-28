#include "imageextractortest.h"
#include <limits>
#include <opencv2/highgui.hpp>

const cv::Scalar BudExtractorTestImage::tpColor = cv::Scalar(0,255,255);
const cv::Scalar BudExtractorTestImage::fpColor = cv::Scalar(255,0,255);
const cv::Scalar BudExtractorTestImage::fnColor = cv::Scalar(0,0,255);

cv::Mat BudExtractorTestImage::getImage() const
{
  return image;
}

void BudExtractorTestImage::setImage(const cv::Mat &value)
{
  image = value;
}

cv::Mat BudExtractorTestImage::getMask() const
{
  return mask;
}

void BudExtractorTestImage::setMask(const cv::Mat &value)
{
  mask = value;
}

bool BudExtractorTestImage::isValid() const
{
  return image.data!=nullptr;
}

std::chrono::high_resolution_clock::duration BudExtractorTestImage::test(BudExtractor &extractor, float positionToleranceFactor, size_t &truePositives, size_t &falsePositives, size_t &falseNegatives, const std::string &outputFile) const
{
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::vector<Circle> prediction = extractor.extractBuds(image,mask);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  cv::Mat circleImage;
  if(!outputFile.empty())
    circleImage = getImage().clone();
  for(size_t i=0;i<budsTruth.size();i++)
  {
    float distance;
    size_t closestIndex = findClosest(budsTruth[i], prediction, distance);
    if((closestIndex >= prediction.size()) || (distance > positionToleranceFactor))
    {
      falseNegatives++;
      if(!outputFile.empty())
      {
        cv::circle(circleImage,budsTruth[i],5,fnColor,-1);
      }
      continue;
    }
    if(!outputFile.empty())
    {
      cv::circle(circleImage,budsTruth[i],3,tpColor,-1);
      cv::circle(circleImage,prediction[closestIndex].center,prediction[closestIndex].radius,tpColor,2);
    }
    prediction.erase(prediction.begin()+closestIndex);
    truePositives++;
  }
  falsePositives += prediction.size();
  if(!outputFile.empty())
  {
    for(size_t i=0;i<prediction.size();i++)
    {
      cv::circle(circleImage,prediction[i].center,prediction[i].radius,fpColor,2);
    }
    cv::imwrite(outputFile,circleImage);
  }
  return t2 - t1;
}

std::vector<cv::Point> BudExtractorTestImage::getBudsTruth() const
{
  return budsTruth;
}

void BudExtractorTestImage::setBudsTruth(const std::vector<cv::Point> &value)
{
  budsTruth = value;
}

std::string BudExtractorTestImage::getFilename() const
{
  return filename;
}

void BudExtractorTestImage::setFilename(const std::string &value)
{
  filename = value;
}

size_t BudExtractorTestImage::findClosest(const cv::Point &point, const std::vector<Circle> &prediction, float& distance) const
{
  size_t minIndex = std::numeric_limits<size_t>::max();
  float minDistance = std::numeric_limits<float>::infinity();
  for(size_t i=0;i<prediction.size();i++)
  {
    distance = std::hypot(point.x-prediction[i].center.x,point.y-prediction[i].center.y)/prediction[i].radius;
    if(distance<minDistance)
    {
      minDistance = distance;
      minIndex = i;
    }
  }
  distance = minDistance;
  return minIndex;
}

BudExtractorTestImage::BudExtractorTestImage()
{

}
