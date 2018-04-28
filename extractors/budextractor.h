#ifndef BUDEXTRACTOR_H
#define BUDEXTRACTOR_H

#include <opencv2/core/core.hpp>
#include "circle.h"

class BudExtractor
{
protected:
  cv::Mat localContrastNormalization(const cv::Mat& input, int radius, double normalizationFactor=1.0f, double offset = 0.0f) const;
public:
  virtual std::vector<Circle> extractBuds(const cv::Mat& image, const cv::Mat& mask) = 0;
  size_t extractBudsNum(const cv::Mat& image, cv::Mat& mask);
  cv::Mat budImage(const cv::Mat& image, cv::Mat& mask);
};

#endif // BUDEXTRACTOR_H
