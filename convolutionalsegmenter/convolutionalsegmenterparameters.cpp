#include "convolutionalsegmenterparameters.h"

ConvolutionalSegmenterParameters::ConvolutionalSegmenterParameters()
{
  this->cropSize = 0;
}

ConvolutionalSegmenterParameters::ConvolutionalSegmenterParameters(int cropSize, const std::string &prototxtPath, const std::string &caffemodelPath)
{
  this->cropSize = cropSize;
  this->prototxtPath = prototxtPath;
  this->caffemodelPath = caffemodelPath;
}

bool ConvolutionalSegmenterParameters::operator==(const ConvolutionalSegmenterParameters &other)
{
  return (this->prototxtPath == other.prototxtPath) && (this->caffemodelPath == other.caffemodelPath);
}

bool ConvolutionalSegmenterParameters::operator!=(const ConvolutionalSegmenterParameters &other)
{
  return !(*this == other);
}
