#ifndef CONVOLUTIONALSEGMENTER_H
#define CONVOLUTIONALSEGMENTER_H

#include <string>
#include <caffe/caffe.hpp>
#include <caffe/net.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <caffe/data_transformer.hpp>
#include <iostream>
#include "convolutionalsegmenterparameters.h"

class ConvolutionalSegmenter
{
  caffe::Net<float>* net;
  caffe::Blob<float> input;
  int height, width;
  int cropSize;
  caffe::DataTransformer<float>* inputTransformer;

  void segment(cv::Mat& dest, const cv::Mat& image, int channel, int x, int y);
  cv::Mat segmentNative(const cv::Mat& image, int channel);
  cv::Mat blobToMat(const caffe::Blob<float>& blob, int channel);
  cv::Mat expandImage(const cv::Mat& image);
  void init(int height, int width);
  ConvolutionalSegmenter();
public:
  ConvolutionalSegmenter(const ConvolutionalSegmenterParameters& parameters);
  ConvolutionalSegmenter(const std::string& prototxtPath, const std::string& caffemodelPath);
  ConvolutionalSegmenter(const std::string& prototxtPath);
  ~ConvolutionalSegmenter();
  void loadCaffeModel(const std::string& caffemodelPath);
  cv::Mat segment(const std::string& filepath, int channel);
  cv::Mat segment(const cv::Mat& image, int channel);
  int getCropSize() const;
  void setCropSize(int value);
};

#endif // CONVOLUTIONALSEGMENTER_H
