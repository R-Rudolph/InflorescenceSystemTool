#ifndef BUDEXTRACTORTEST_H
#define BUDEXTRACTORTEST_H

#include <QList>
#include <QString>
#include "budextractor.h"
#include "imageextractortest.h"
#include <chrono>

#include "convolutionalsegmenter/convolutionalsegmenterparameters.h"

class BudExtractorTest
{
public:
  enum SegmentationOptions
  {
    SEGMENTATION_ANNOTATED,
    SEGMENTATION_CNN,
    SEGMENTATION_NONE
  };
private:
  bool settingsChanged;
  SegmentationOptions currentSegmentation;
  ConvolutionalSegmenterParameters convolutionalSegmenterParameters;

  float positionToleranceFactor;
  QList<BudExtractorTestImage> imageTesters;
  QString lastInfoPath;
  size_t truePositives;
  size_t falsePositives;
  size_t falseNegatives;
  double meanDeviationValue;
  double deviationStdDeviationValue;

  double precision(size_t truePositives, size_t falsePositives, size_t falseNegatives);
  double recall(size_t truePositives, size_t falsePositives, size_t falseNegatives);
  double f1Score(size_t truePositives, size_t falsePositives, size_t falseNegatives);
  double fScore(size_t truePositives, size_t falsePositives, size_t falseNegatives, float beta);
public:
  BudExtractorTest();
  bool loadInfos(const QString& path, bool force=false);
  float getPositionToleranceFactor() const;
  void setPositionToleranceFactor(float value);
  QString getLastInfoPath() const;
  std::chrono::high_resolution_clock::duration test(BudExtractor& extractor, const QString& outputDirPath=QString());
  double precision();
  double recall();
  double f1Score();
  double fScore(float beta);
  double meanDeviation();
  double getDeviationStdDeviationValue() const;
  void setDeviationStdDeviationValue(double value);
  SegmentationOptions getCurrentSegmentation() const;
  void setCurrentSegmentation(const SegmentationOptions &value);
  ConvolutionalSegmenterParameters getConvolutionalSegmenterParameters() const;
  void setConvolutionalSegmenterParameters(const ConvolutionalSegmenterParameters &value);
};

#endif // BUDEXTRACTORTEST_H
