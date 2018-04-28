#include "inflorescencecollection.h"
#include <iostream>

void InflorescenceCollection::regionGrow(cv::Point2i point, const cv::Mat& segmentation)
{
  if(regionMap.at<int32_t>(point)!=-1)
    return;
  std::list<cv::Point> queue;
  queue.push_back(point);
  int regionSizeCtr = 0;
  while(queue.size()>0)
  {
    cv::Point p = queue.front();
    queue.pop_front();
    if(regionMap.at<int32_t>(p)==-1)
    {
      regionMap.at<int32_t>(p) = regionIndex;
      regionSizeCtr++;
      pushBackIfValid(queue,p+cv::Point(1,0),segmentation);
      pushBackIfValid(queue,p+cv::Point(-1,0),segmentation);
      pushBackIfValid(queue,p+cv::Point(0,1),segmentation);
      pushBackIfValid(queue,p+cv::Point(0,-1),segmentation);
    }
  }
  regionSizes.push_back(regionSizeCtr);
  regionIndex++;
}

void InflorescenceCollection::pushBackIfValid(std::list<cv::Point>& queue, cv::Point point, const cv::Mat& segmentation)
{
  if(point.x>=0 && point.y >=0 && point.x<regionMap.cols && point.y<regionMap.rows && segmentation.at<uchar>(point)>0)
    queue.push_back(point);
}

InflorescenceCollection::InflorescenceCollection(const cv::Mat& segmentation)
{
  regionIndex = 0;
  if(segmentation.type()==CV_8U)
  {
    regionMap = cv::Mat(segmentation.rows,segmentation.cols,CV_32S);
    regionMap = -1;
    std::vector<cv::Point> nonZeros;
    cv::findNonZero(segmentation,nonZeros);
    for(size_t i=0;i<nonZeros.size();i++)
    {
      regionGrow(nonZeros[i],segmentation);
    }
    regionContents = std::vector<int>(regionSizes.size(),0);
  }
}

void InflorescenceCollection::addContent(const std::vector<Circle>& circles)
{
  for(size_t i=0;i<circles.size();i++)
    addContent(circles[i]);
}

void InflorescenceCollection::addContent(const Circle& circle)
{
  int regionID = regionMap.at<int32_t>(circle.center);
  if(regionID!=-1)
  {
    regionContents[regionID]++;
  }
}

void InflorescenceCollection::stats()
{
  std::cout << "Regions: " << regionIndex << std::endl;
  for(int i=0;i<regionIndex;i++)
  {
    std::cout << i << " -> " << "size:" << regionSizes[i] << ", content:" << regionContents[i] << std::endl;
  }
}

int InflorescenceCollection::regionsCount()
{
  return regionIndex;
}

int InflorescenceCollection::regionSize(int region)
{
  if(region<regionIndex)
    return regionSizes[region];
  return -1;
}

int InflorescenceCollection::regionContent(int region)
{
  if(region<regionIndex)
    return regionContents[region];
  return -1;
}
