#ifndef CONFUSIONMATRIX_H
#define CONFUSIONMATRIX_H

#include<vector>
#include <opencv2/imgproc.hpp>

class ConfusionMatrix
{
  std::vector<std::vector<size_t> > matrix;
  std::vector<size_t> predictions;
  std::vector<size_t> labels;
  size_t ctr;
  size_t numClasses;
public:
  ConfusionMatrix(size_t numClasses);
  void increment(size_t predictionClass, size_t actualClass);
  double accuracy() const;
  double jaccardIndex(size_t classIndex) const;
  double meanJaccardIndex() const;
  void clear();
  void setNumClasses(size_t numClasses);
  size_t getNumClasses() const;
  size_t getValue(size_t predictionClass, size_t actualClass);

  void addImage(const cv::Mat& truth, const cv::Mat& prediction);
};

#endif // CONFUSIONMATRIX_H
