#include "confusionmatrix.h"
#include<iostream>
ConfusionMatrix::ConfusionMatrix(size_t numClasses)
{
  setNumClasses(numClasses);
}

void ConfusionMatrix::increment(size_t predictionClass, size_t actualClass)
{
  predictions[predictionClass]++;
  labels[actualClass]++;
  matrix[actualClass][predictionClass]++;
  ctr++;
}

void ConfusionMatrix::clear()
{
  ctr = 0;
  for(size_t i=0;i<numClasses;i++)
  {
    predictions[numClasses] = 0;
    labels[numClasses] = 0;
    for(size_t j=0;j<numClasses;j++)
    {
      matrix[i][j] = 0;
    }
  }
}

void ConfusionMatrix::setNumClasses(size_t numClasses)
{
  ctr = 0;
  this->numClasses = numClasses;
  matrix.clear();
  predictions = std::vector<size_t>(numClasses,0);
  labels = std::vector<size_t>(numClasses,0);
  for(size_t i=0;i<numClasses;i++)
    matrix.push_back(std::vector<size_t>(numClasses,0));
}

size_t ConfusionMatrix::getNumClasses() const
{
  return numClasses;
}

size_t ConfusionMatrix::getValue(size_t predictionClass, size_t actualClass)
{
  return matrix[actualClass][predictionClass];
}

void ConfusionMatrix::addImage(const cv::Mat &truth, const cv::Mat &prediction)
{
  int nRows = truth.rows;
  int nCols = truth.cols;
  if (truth.isContinuous() && prediction.isContinuous())
  {
    nCols *= nRows;
    nRows = 1;
  }
  int x,y;
  const uchar* pTruth;
  const uchar* pPrediction;
  for( y = 0; y < nRows; ++y)
  {
    pTruth      = truth.ptr<uchar>(y);
    pPrediction = prediction.ptr<uchar>(y);
    for ( x = 0; x < nCols; ++x)
    {
      increment(pPrediction[x],pTruth[x]);
    }
  }
}

double ConfusionMatrix::accuracy() const
{
  size_t sum = 0;
  for(size_t i=0;i<numClasses;i++)
  {
    sum += matrix[i][i];
  }
  return ((double)sum)/((double)ctr);
}

double ConfusionMatrix::jaccardIndex(size_t classIndex) const
{
  double intersectionCard = matrix[classIndex][classIndex];
  double unionCard        = predictions[classIndex]+labels[classIndex]-intersectionCard;
  return intersectionCard/unionCard;
}

double ConfusionMatrix::meanJaccardIndex() const
{
  double mean = 0.0;
  for(size_t i=0;i<numClasses;i++)
  {
    mean += jaccardIndex(i);
  }
  mean /= numClasses;
  return mean;
}
