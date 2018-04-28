#include "convolutionalsegmenter.h"

#include <opencv2/imgproc.hpp>

int ConvolutionalSegmenter::getCropSize() const
{
  return cropSize;
}

void ConvolutionalSegmenter::setCropSize(int value)
{
  cropSize = value;
}

cv::Mat ConvolutionalSegmenter::segmentNative(const cv::Mat &image, int channel)
{
  inputTransformer->Transform(image,&input);
  float loss;
  net->input_blobs()[0]->CopyFrom(input);
  std::vector<caffe::Blob<float>*> top;
  top = net->Forward(&loss);
  caffe::Blob<float>& out = *top[0];
  cv::Mat result = blobToMat(out,channel);
  cv::resize(result,result,cv::Size(image.cols,image.rows));
  return result;
}

cv::Mat ConvolutionalSegmenter::blobToMat(const caffe::Blob<float> &blob, int channel)
{
  cv::Mat result(blob.height(),blob.width(),CV_32F);
  for(int x=0;x<blob.width();x++)
  {
    for(int y=0;y<blob.height();y++)
    {
      result.at<float>(x,y) = blob.data_at(0,channel,x,y);
    }
  }
  return result;
}

cv::Mat ConvolutionalSegmenter::expandImage(const cv::Mat &image)
{
  cv::Mat expandedImage(image.rows+cropSize*2,image.cols+cropSize*2,image.type());
  image.copyTo(expandedImage(cv::Rect(cropSize,cropSize,image.cols,image.rows)));
  //expand by horizontal sides
  {
    cv::Mat sideHorizontal(image.rows,cropSize,image.type());
    //copy left side and flip horizontally
    image(cv::Rect(0,0,cropSize,image.rows)).copyTo(sideHorizontal);
    cv::flip(sideHorizontal,sideHorizontal,1);
    sideHorizontal.copyTo(expandedImage(cv::Rect(0,cropSize,cropSize,image.rows)));
    //copy right side and flip horizontally
    image(cv::Rect(image.cols-cropSize,0,cropSize,image.rows)).copyTo(sideHorizontal);
    cv::flip(sideHorizontal,sideHorizontal,1);
    sideHorizontal.copyTo(expandedImage(cv::Rect(image.cols+cropSize,cropSize,cropSize,image.rows)));
  }
  //expand by vertical sides
  {
    cv::Mat sideVertical(cropSize,image.cols,image.type());
    //copy top side and flip horizontally left side
    image(cv::Rect(0,0,image.cols,cropSize)).copyTo(sideVertical);
    cv::flip(sideVertical,sideVertical,0);
    sideVertical.copyTo(expandedImage(cv::Rect(cropSize,0,image.cols,cropSize)));
    //copy bottom side and flip horizontally left side
    image(cv::Rect(0,image.rows-cropSize,image.cols,cropSize)).copyTo(sideVertical);
    cv::flip(sideVertical,sideVertical,0);
    sideVertical.copyTo(expandedImage(cv::Rect(cropSize,image.rows+cropSize,image.cols,cropSize)));
  }
  //copy corners
  {
    cv::Mat corner(cropSize,cropSize,image.type());
    //top left
    image(cv::Rect(0,0,cropSize,cropSize)).copyTo(corner);
    cv::flip(corner,corner,-1);
    corner.copyTo(expandedImage(cv::Rect(0,0,cropSize,cropSize)));
    //top right
    image(cv::Rect(image.cols-cropSize,0,cropSize,cropSize)).copyTo(corner);
    cv::flip(corner,corner,-1);
    corner.copyTo(expandedImage(cv::Rect(image.cols+cropSize,0,cropSize,cropSize)));
    //bottom left
    image(cv::Rect(0,image.rows-cropSize,cropSize,cropSize)).copyTo(corner);
    cv::flip(corner,corner,-1);
    corner.copyTo(expandedImage(cv::Rect(0,image.rows+cropSize,cropSize,cropSize)));
    //bottom right
    image(cv::Rect(image.cols-cropSize,image.rows-cropSize,cropSize,cropSize)).copyTo(corner);
    cv::flip(corner,corner,-1);
    corner.copyTo(expandedImage(cv::Rect(image.cols+cropSize,image.rows+cropSize,cropSize,cropSize)));
  }
  return expandedImage;
}

void ConvolutionalSegmenter::init(int height, int width)
{
  this->height = height;
  this->width = width;
  cropSize = 0;
  caffe::TransformationParameter inputTransformParameter;
  inputTransformParameter.set_scale(1);
  delete inputTransformer;
  inputTransformer = new caffe::DataTransformer<float>(inputTransformParameter, caffe::TEST);
  input.Reshape(1,3,height,width);
}

void ConvolutionalSegmenter::segment(cv::Mat &dest, const cv::Mat &image, int channel, int x, int y)
{
  cv::Mat out = segmentNative(image(cv::Rect(x,y,width,height)), channel);
  const cv::Mat& destRegion = dest(cv::Rect(x+cropSize,y+cropSize,width-cropSize*2,height-cropSize*2));
  out(cv::Rect(cropSize,cropSize,width-cropSize*2,height-cropSize*2)).copyTo(destRegion);
}

ConvolutionalSegmenter::ConvolutionalSegmenter()
{
  this->net = nullptr;
  this->inputTransformer = nullptr;
}

ConvolutionalSegmenter::ConvolutionalSegmenter(const ConvolutionalSegmenterParameters &parameters)
  : ConvolutionalSegmenter(parameters.prototxtPath,parameters.caffemodelPath)
{
  setCropSize(parameters.cropSize);
}

ConvolutionalSegmenter::ConvolutionalSegmenter(const std::string &prototxtPath, const std::string &caffemodelPath)
  : ConvolutionalSegmenter(prototxtPath)
{
  loadCaffeModel(caffemodelPath);
}

ConvolutionalSegmenter::ConvolutionalSegmenter(const std::string &prototxtPath)
  : ConvolutionalSegmenter()
{
  caffe::NetParameter nparameter;
  caffe::ReadNetParamsFromTextFileOrDie(prototxtPath,&nparameter);
  net = new caffe::Net<float>(nparameter);
  init(nparameter.layer(0).input_param().shape(0).dim(2),nparameter.layer(0).input_param().shape(0).dim(2));
}

ConvolutionalSegmenter::~ConvolutionalSegmenter()
{
  delete net;
  delete inputTransformer;
}

void ConvolutionalSegmenter::loadCaffeModel(const std::string &caffemodelPath)
{
  caffe::NetParameter valueParameter;
  caffe::ReadNetParamsFromBinaryFileOrDie(caffemodelPath,&valueParameter);
  net->CopyTrainedLayersFrom(valueParameter);
}

cv::Mat ConvolutionalSegmenter::segment(const std::string &filepath, int channel)
{
  cv::Mat image = cv::imread(filepath);
  return segment(image, channel);
}

cv::Mat ConvolutionalSegmenter::segment(const cv::Mat &image, int channel)
{
  cv::Mat expandedImage = expandImage(image);
  cv::Mat segmentation = cv::Mat::zeros(expandedImage.rows,expandedImage.cols,CV_32F);
  int x,y;
  for(x=0;x<expandedImage.cols-width;x+=width-cropSize*2)
  {
    for(y=0;y<expandedImage.rows-height;y+=height-cropSize*2)
    {
      segment(segmentation, expandedImage, channel, x, y);
    }
    y = expandedImage.rows-height;
    segment(segmentation, expandedImage, channel, x, y);
  }
  x = expandedImage.cols-width;
  for(y=0;y<expandedImage.rows-height;y+=height-cropSize*2)
  {
    segment(segmentation, expandedImage, channel, x, y);
  }
  y = expandedImage.rows-height;
  segment(segmentation, expandedImage, channel, x, y);
  return segmentation(cv::Rect(cropSize,cropSize,image.cols,image.rows));
}
