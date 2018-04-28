#include "cannyoperator.h"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>

const cv::Mat& CannyOperator::getEdges() const
{
  return thinnedMagnitude;
}

const cv::Mat& CannyOperator::getGradientDirection() const
{
  return gradientDirection;
}

int CannyOperator::getThresholdLow() const
{
  return thresholdLow;
}

int &CannyOperator::getThresholdLowRef()
{
  return thresholdLow;
}

void CannyOperator::setThresholdLow(int value)
{
  thresholdLow = value;
}

int CannyOperator::getThresholdHigh() const
{
  return thresholdHigh;
}

int &CannyOperator::getThresholdHighRef()
{
  return thresholdHigh;
}

void CannyOperator::setThresholdHigh(int value)
{
  thresholdHigh = value;
}

const cv::Mat& CannyOperator::getMask() const
{
  return mask;
}

bool CannyOperator::greaterMagnitude(int row1, int col1, int row2, int col2)
{
  return (gradientMagnitude.at<float>(row1,col1) > gradientMagnitude.at<float>(row2,col2));
  return (directionValue(row1,col1) != directionValue(row2,col2))
         || (gradientMagnitude.at<float>(row1,col1) > gradientMagnitude.at<float>(row2,col2));
}

bool CannyOperator::greaterEqualMagnitude(int row1, int col1, int row2, int col2)
{
  return (gradientMagnitude.at<float>(row1,col1) >= gradientMagnitude.at<float>(row2,col2));
  return (directionValue(row1,col1) != directionValue(row2,col2))
         || (gradientMagnitude.at<float>(row1,col1) >= gradientMagnitude.at<float>(row2,col2));
}

void CannyOperator::nonMaxSuppression(int row, int col, std::list<cv::Point>& weakEdges)
{
  bool keep = false;
  switch(directionValue(row,col))
  {
    //east <-> west
    case 0:
      if(greaterMagnitude(row,col,row,col-1) && greaterEqualMagnitude(row,col,row,col+1))
        keep = true;
      break;
    //north-west <-> south-east
    case 1:
      if(greaterMagnitude(row,col,row-1,col-1) && greaterEqualMagnitude(row,col,row+1,col+1))
        keep = true;
      break;
    //north <-> south
    case 2:
      if(greaterMagnitude(row,col,row-1,col) && greaterEqualMagnitude(row,col,row+1,col))
        keep = true;
      break;
    //north-east <-> south-west
    case 3:
      if(greaterMagnitude(row,col,row-1,col+1) && greaterEqualMagnitude(row,col,row+1,col-1))
        keep = true;
      break;
    default:
      break;
  }
  if(keep)
  {
    float value = gradientMagnitude.at<float>(row,col);
    if(value>=thresholdHigh)
      thinnedMagnitude.at<unsigned char>(row,col) = EDGE_STRONG;
    else if(value>=thresholdLow)
    {
      thinnedMagnitude.at<unsigned char>(row,col) = EDGE_WEAK;
      weakEdges.push_back(cv::Point(col,row));
    }
  }
}

void CannyOperator::expandList(const cv::Point &point, std::vector<cv::Point> &list, bool &foundStrong)
{
  unsigned char& cell = thinnedMagnitude.at<unsigned char>(point);
  switch(cell)
  {
    case EDGE_WEAK:
      cell = EDGE_NONE;
      list.push_back(point);
      break;
    case EDGE_STRONG:
      foundStrong = true;
      break;
    default:
      break;
  }
}

void CannyOperator::trackEdge(const cv::Point& edgePoint)
{
  std::vector<cv::Point> adjacentPoints;
  adjacentPoints.reserve(100);
  bool setStrong = false;
  expandList(edgePoint,adjacentPoints,setStrong);
  for(size_t i=0;i<adjacentPoints.size() && !setStrong;i++)
  {
    //handle 8-neighborhood
    expandList(adjacentPoints[i] + cv::Point(-1,-1),adjacentPoints,setStrong);
    expandList(adjacentPoints[i] + cv::Point(-1, 0),adjacentPoints,setStrong);
    expandList(adjacentPoints[i] + cv::Point(-1, 1),adjacentPoints,setStrong);
    expandList(adjacentPoints[i] + cv::Point( 0,-1),adjacentPoints,setStrong);
    expandList(adjacentPoints[i] + cv::Point( 0, 1),adjacentPoints,setStrong);
    expandList(adjacentPoints[i] + cv::Point( 1,-1),adjacentPoints,setStrong);
    expandList(adjacentPoints[i] + cv::Point( 1, 0),adjacentPoints,setStrong);
    expandList(adjacentPoints[i] + cv::Point( 1, 1),adjacentPoints,setStrong);
  }
  if(setStrong)
  {
    //if strong edge was found, set all found weak edges to strong edges
    //otherwise, they remain None-Edges, as set in the expandList-Function
    for(size_t i=0;i<adjacentPoints.size();i++)
    {
      thinnedMagnitude.at<unsigned char>(adjacentPoints[i]) = EDGE_STRONG;
    }
  }
}

int CannyOperator::directionValue(int row, int col)
{
  return ((int)std::round(gradientDirection.at<float>(row,col)*4.0/M_PI))%4;
}

CannyOperator::CannyOperator(int thresholdLow, int thresholdHigh)
{
  this->thresholdLow = thresholdLow;
  this->thresholdHigh = thresholdHigh;
}

void CannyOperator::processImage(const cv::Mat &image, const cv::Mat& mask)
{
  this->mask = mask;
  gradientDirection = cv::Mat::zeros(image.rows,image.cols,CV_32F);
  gradientMagnitude = cv::Mat::zeros(image.rows,image.cols,CV_32F);
  thinnedMagnitude = cv::Mat::zeros(image.rows,image.cols,CV_8U);

  cv::Mat sobelX = cv::Mat::zeros(image.rows,image.cols,CV_32F);
  cv::Mat sobelY = cv::Mat::zeros(image.rows,image.cols,CV_32F);
  cv::Mat imageGray;
  if(image.channels()==3)
    cv::cvtColor(image,imageGray,CV_BGR2GRAY);
  else
    imageGray = image;
  //apply sobel in both directions
  cv::Sobel(imageGray,sobelX,CV_32F,1,0,3);
  cv::Sobel(imageGray,sobelY,CV_32F,0,1,3);
  //find Magnitude and Direction for each point
  for(int row=0;row<image.rows;row++)
  {
    for(int col=0;col<image.cols;col++)
    {
      float dx = sobelX.at<float>(row,col);
      float dy = sobelY.at<float>(row,col);
      gradientMagnitude.at<float>(row,col) = std::hypot(dx,dy);
      float rad = std::atan2(dy,dx);
      //if(rad<0.0)
      rad += M_PI;
      gradientDirection.at<float>(row,col) = rad;
    }
  }
  cv::Canny(imageGray,thinnedMagnitude,thresholdLow,thresholdHigh,3);
  //cv::threshold(gradientMagnitude,thinnedMagnitude,thresholdLow,255,cv::THRESH_BINARY);
  //thinnedMagnitude.convertTo(thinnedMagnitude,CV_8U);
  cv::bitwise_and(thinnedMagnitude,mask,thinnedMagnitude);
  /*

  std::list<cv::Point> weakEdges;
  //Non-maximum suppression and thresholding
  for(int row=1;row<image.rows-1;row++)
  {
    for(int col=1;col<image.cols-1;col++)
    {
      if(mask.at<unsigned char>(row,col)==0)
        continue;
      nonMaxSuppression(row,col,weakEdges);
    }
  }
  //Edge Tracking
  for(auto it = weakEdges.begin(); it != weakEdges.end(); ++it)
  {
    trackEdge(*it);
  }
  */
  cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2,2));
  cv::dilate(thinnedMagnitude,thinnedMagnitude,element);
}

void CannyOperator::processImage(const cv::Mat &image)
{
  processImage(image,cv::Mat::ones(image.rows,image.cols,CV_8U));
}
