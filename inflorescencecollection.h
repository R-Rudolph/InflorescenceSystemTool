#ifndef INFLORESCENCECOLLECTION_H
#define INFLORESCENCECOLLECTION_H

#include <opencv2/core.hpp>
#include <list>
#include "circle.h"

class InflorescenceCollection
{
  int regionIndex;
  cv::Mat regionMap;
  std::vector<int> regionSizes;
  std::vector<int> regionContents;
  void regionGrow(cv::Point point, const cv::Mat& segmentation);
  void pushBackIfValid(std::list<cv::Point>& queue, cv::Point point, const cv::Mat& segmentation);
public:
  InflorescenceCollection(const cv::Mat& segmentation);
  void addContent(const std::vector<Circle>& circles);
  void addContent(const Circle& circle);
  void stats();
  int regionsCount();
  int regionSize(int region);
  int regionContent(int region);
};

#endif // INFLORESCENCECOLLECTION_H
