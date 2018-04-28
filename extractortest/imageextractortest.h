#ifndef IMAGEEXTRACTORTEST_H
#define IMAGEEXTRACTORTEST_H

#include <chrono>
#include <opencv2/imgproc.hpp>
#include "budextractor.h"

class BudExtractorTestImage
{
  static const cv::Scalar tpColor;
  static const cv::Scalar fpColor;
  static const cv::Scalar fnColor;
  cv::Mat image;
  cv::Mat mask;
  std::string filename;
  std::vector<cv::Point> budsTruth;
  size_t findClosest(const cv::Point& point, const std::vector<Circle>& prediction, float& distance) const;
public:
  BudExtractorTestImage();
  cv::Mat getImage() const;
  void setImage(const cv::Mat &value);
  cv::Mat getMask() const;
  void setMask(const cv::Mat &value);
  std::vector<cv::Point> getBudsTruth() const;
  void setBudsTruth(const std::vector<cv::Point> &value);

  bool isValid() const;
  std::chrono::high_resolution_clock::duration test(BudExtractor& extractor,
                                                    float positionToleranceFactor,
                                                    size_t& truePositives,
                                                    size_t& falsePositives,
                                                    size_t& falseNegatives,
                                                    const std::string& outputFile="") const;
  std::string getFilename() const;
  void setFilename(const std::string &value);
};

#endif // IMAGEEXTRACTORTEST_H
