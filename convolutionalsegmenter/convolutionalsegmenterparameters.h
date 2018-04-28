#ifndef CONVOLUTIONALSEGMENTERPARAMETERS_H
#define CONVOLUTIONALSEGMENTERPARAMETERS_H

#include <string>

struct ConvolutionalSegmenterParameters
{
  int cropSize;
  std::string prototxtPath;
  std::string caffemodelPath;

  ConvolutionalSegmenterParameters();
  ConvolutionalSegmenterParameters(int cropSize, const std::string& prototxtPath, const std::string& caffemodelPath);
  bool operator==(const ConvolutionalSegmenterParameters& other);
  bool operator!=(const ConvolutionalSegmenterParameters& other);
};

#endif // CONVOLUTIONALSEGMENTERPARAMETERS_H
