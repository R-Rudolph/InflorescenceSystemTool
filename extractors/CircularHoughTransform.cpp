#include "CircularHoughTransform.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
float CircularHoughTransform::getThreshold() const
{
  return threshold;
}

float &CircularHoughTransform::getThresholdRef()
{
  return threshold;
}

void CircularHoughTransform::setThreshold(float value)
{
  threshold = value;
}

std::vector<Circle> CircularHoughTransform::extractCircles(const CannyOperator &image)
{
  std::vector<Circle> result;
  if(isValid())
  {
    cv::Mat mask = cv::Mat::zeros(image.getEdges().rows,image.getEdges().cols,CV_8U);
    std::vector< Circle > circles;
    for(int radius=maxRadius;radius>=minRadius;radius--)
    {
      std::vector< Circle > newCircles = extractCirclesForRadius(radius,image);
      circles.insert(circles.end(),newCircles.begin(),newCircles.end());
    }
    std::sort(circles.begin(),circles.end());
    for(auto it = circles.begin();it!=circles.end();++it)
    {
      if(mask.at<unsigned char>(it->center)==0)
      {
        cv::circle(mask,it->center,it->radius*adjacentBlockingFactor,cv::Scalar(255),-1);
        result.push_back(Circle(it->center,it->radius));
      }
    }
  }
  return result;
}

float CircularHoughTransform::getAdjacentBlockingFactor() const
{
  return adjacentBlockingFactor;
}

float &CircularHoughTransform::getAdjacentBlockingFactorRef()
{
  return adjacentBlockingFactor;
}

void CircularHoughTransform::setAdjacentBlockingFactor(float value)
{
  adjacentBlockingFactor = value;
}

std::vector<Circle> CircularHoughTransform::extractCirclesForRadius(int radius, const CannyOperator &image)
{
  std::vector< Circle > result;
  int rows = image.getEdges().rows;
  int cols = image.getEdges().cols;
  cv::Mat houghSpace = cv::Mat::zeros(rows,cols,CV_32F);
  cv::Mat kernel = cv::Mat::zeros(radius*2+1,radius*2+1,CV_32F);
  cv::circle(kernel,cv::Point(radius,radius),radius,1);
  kernel = kernel / cv::sum(kernel)[0];
  //cv::filter2D(image.getEdges(),houghSpace,CV_32F,kernel);
  for(int row=0;row<rows;row++)
  {
    for(int col=0;col<cols;col++)
    {
      if(image.getEdges().at<unsigned char>(row,col)==255)
      {
        cd.drawPartial(Circle(cv::Point(col,row),radius),image.getGradientDirection().at<float>(row,col),houghSpace);
      }
    }
  }
  cv::Mat thresholdMap;
  cv::threshold(houghSpace,thresholdMap,threshold*cd.numPoints(radius),1.0,cv::THRESH_BINARY);
  //cv::threshold(houghSpace,thresholdMap,threshold*255,1.0,cv::THRESH_BINARY);
  cv::Mat maxMap;
  cv::dilate(thresholdMap, maxMap, cv::Mat());
  cv::compare(thresholdMap, maxMap, maxMap, cv::CMP_GE);
  {
    cv::Mat non_plateau_mask;
    cv::erode(thresholdMap, non_plateau_mask, cv::Mat());
    cv::compare(thresholdMap, non_plateau_mask, non_plateau_mask, cv::CMP_GT);
    cv::bitwise_and(maxMap, non_plateau_mask, maxMap);
  }
  std::vector<cv::Point> nonZero;
  cv::findNonZero(maxMap,nonZero);
  //shrink regions
  for(auto it = nonZero.begin();it!=nonZero.end();++it)
  {
    if(maxMap.at<unsigned char>(*it)!=0)
    {
      result.push_back(Circle(findRegionCenter(maxMap,*it),radius,houghSpace.at<float>(*it)/cd.numPoints(radius)));
      //result.push_back(Circle(*it,radius,houghSpace.at<float>(*it)/cd.numPoints(radius)));
    }
  }
  /*
  for(auto it = nonZero.begin();it!=nonZero.end();++it)
  {
    result.push_back(Circle(*it,radius));
  }
  */
  /*
  for(int row=0;row<rows;row++)
  {
    for(int col=0;col<cols;col++)
    {
      float value = houghSpace.at<float>(row,col) / cd.numPoints(radius);
      if(value>threshold && isLocalMaxiumum(houghSpace,col,row))
      {
        result.push_back(Circle(cv::Point(col,row),radius));
      }
    }
  }
  */
  //cv::imshow("test",maxMap);
  //cv::waitKey(0);
  return result;
}

bool CircularHoughTransform::isLocalMaxiumum(cv::Mat &houghSpace, int col, int row)
{
  float value = houghSpace.at<float>(row,col);
  if(value >= houghSpace.at<float>(row-1,col-1) &&
     value >= houghSpace.at<float>(row-1,col) &&
     value >= houghSpace.at<float>(row-1,col+1) &&
     value >= houghSpace.at<float>(row,col-1) &&
     value >= houghSpace.at<float>(row,col+1) &&
     value >= houghSpace.at<float>(row+1,col-1) &&
     value >= houghSpace.at<float>(row+1,col) &&
     value >= houghSpace.at<float>(row+1,col+1))
    return true;
  else
    return false;
}

void CircularHoughTransform::addIfNotZero(cv::Mat &map, std::vector<cv::Point> &list, const cv::Point &point)
{
  unsigned char& value = map.at<unsigned char>(point);
  if(value!=0)
  {
    value = 0;
    list.push_back(point);
  }
}

cv::Point CircularHoughTransform::findRegionCenter(cv::Mat &map, const cv::Point &point)
{
  std::vector<cv::Point> points;
  addIfNotZero(map,points,point);
  cv::Point result(0,0);
  for(size_t i=0;i<points.size();i++)
  {
    cv::Point p = points[i];
    addIfNotZero(map,points,cv::Point(p.x,p.y-1));
    addIfNotZero(map,points,cv::Point(p.x-1,p.y));
    addIfNotZero(map,points,cv::Point(p.x+1,p.y));
    addIfNotZero(map,points,cv::Point(p.x,p.y+1));
    result += p;
  }
  if(points.size()!=0)
  {
    return cv::Point(result.x/points.size(),result.y/points.size());
  }
  else
  {
    return point;
  }
}

CircularHoughTransform::CircularHoughTransform(int minRadius, int maxRadius) : cd(minRadius,maxRadius)
{
  threshold = .25;
  adjacentBlockingFactor = 1.5;
  this->minRadius = minRadius;
  this->maxRadius = maxRadius;
}

bool CircularHoughTransform::isValid()
{
  return minRadius<=maxRadius && minRadius>0 && maxRadius>0;
}
