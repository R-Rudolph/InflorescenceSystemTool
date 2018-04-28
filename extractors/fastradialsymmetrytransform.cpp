#include "fastradialsymmetrytransform.h"
#include <opencv2/imgproc.hpp>
#include <list>

float FastRadialSymmetryTransform::getRadialStrictness() const
{
  return radialStrictness;
}

void FastRadialSymmetryTransform::setRadialStrictness(float value)
{
  radialStrictness = value;
}

std::vector<int> FastRadialSymmetryTransform::getRanges() const
{
  return ranges;
}

void FastRadialSymmetryTransform::setRanges(const std::vector<int> &value)
{
  ranges = value;
}

float FastRadialSymmetryTransform::getGradientThreshold() const
{
  return magnitudeThreshold;
}

void FastRadialSymmetryTransform::setGradientThreshold(float value)
{
  magnitudeThreshold = value;
}

float FastRadialSymmetryTransform::getCircleSuppressionFactor() const
{
  return circleSuppressionFactor;
}

void FastRadialSymmetryTransform::setCircleSuppressionFactor(float value)
{
  circleSuppressionFactor = value;
}

float FastRadialSymmetryTransform::getGaussianStdDev() const
{
  return gaussianStdDev;
}

void FastRadialSymmetryTransform::setGaussianStdDev(float value)
{
  gaussianStdDev = value;
}

cv::Mat FastRadialSymmetryTransform::transform(const cv::Mat &gradient, int n, const cv::Mat &mask) const
{
  cv::Mat orientationProjection = cv::Mat::zeros(gradient.rows+2*n,gradient.cols+2*n,CV_32F); // O
  cv::Mat magnitudeProjection   = cv::Mat::zeros(gradient.rows+2*n,gradient.cols+2*n,CV_32F); // M
  //construct orientationProjection (O) and magnitudeProjection (M)
  cv::Point p, pOrig;
  cv::Point pointDiff;
  cv::Point pPos;
  cv::Point pNeg;
  if(mask.data == nullptr)
  {
    //mask empty - ignore
    for(int x=n;x<gradient.cols-n;x++)
    {
      for(int y=n;y<gradient.rows-n;y++)
      {
        p = cv::Point(x,y);
        const cv::Vec3f& gradientValue = gradient.at<cv::Vec3f>(p+cv::Point(-n,-n));
        if(gradientValue[2]==0)
          continue;
        pointDiff.x = std::round(gradientValue[0]*n);
        pointDiff.y = std::round(gradientValue[1]*n);
        pPos = p + pointDiff;
        pNeg = p - pointDiff;
        orientationProjection.at<float>(pPos) += 1;
        magnitudeProjection.at<float>(pPos)   += gradientValue[2];
        orientationProjection.at<float>(pNeg) -= 1;
        magnitudeProjection.at<float>(pNeg)   -= gradientValue[2];
      }
    }
  }
  else
  {
    //mask given - check
    for(int x=n;x<gradient.cols-n;x++)
    {
      for(int y=n;y<gradient.rows-n;y++)
      {
        p = cv::Point(x,y);
        pOrig = p + cv::Point(-n,-n);
        if(mask.at<uchar>(pOrig)==0)
          continue;
        const cv::Vec3f& gradientValue = gradient.at<cv::Vec3f>(pOrig);
        if(gradientValue[2]==0)
          continue;
        pointDiff.x = std::round(gradientValue[0]*n);
        pointDiff.y = std::round(gradientValue[1]*n);
        pPos = p + pointDiff;
        pNeg = p - pointDiff;
        orientationProjection.at<float>(pPos) += 1;
        magnitudeProjection.at<float>(pPos)   += gradientValue[2];
        orientationProjection.at<float>(pNeg) -= 1;
        magnitudeProjection.at<float>(pNeg)   -= gradientValue[2];
      }
    }
  }
  orientationProjection = orientationProjection ( cv::Rect(n,n,orientationProjection.cols-2*n, orientationProjection.rows-2*n) );
  magnitudeProjection   = magnitudeProjection( cv::Rect(n,n,magnitudeProjection.cols-2*n,  magnitudeProjection.rows-2*n)   );
  double max;
  //convert O to O tilde
  cv::minMaxIdx(orientationProjection,nullptr,&max);
  orientationProjection /= max;
  //convert M to M tilde
  cv::minMaxIdx(magnitudeProjection,nullptr,&max);
  magnitudeProjection /= max;
  //calculate F (here: S)
  cv::Mat S(gradient.rows,gradient.cols,CV_32F);
  cv::pow(orientationProjection,radialStrictness,S);
  S = S.mul(magnitudeProjection);
  //apply gaussian (A)
  int gaussianSize = std::ceil(0.15*(gaussianStdDev*n + 1.7));
  if(gaussianSize%2 == 0)
    gaussianSize++;
  cv::GaussianBlur(S,S,cv::Size(gaussianSize,gaussianSize),gaussianStdDev*n);
  //done
  return S;
}

bool FastRadialSymmetryTransform::pointIsOk(const cv::Point &p, const cv::Mat &image) const
{
  return (p.x >= 0) &&
         (p.x < image.cols) &&
         (p.y >= 0) &&
         (p.y < image.rows);
}

void FastRadialSymmetryTransform::nonMaximaSuppression(const cv::Mat &image, cv::Mat &maxMap) const
{
  cv::dilate(image, maxMap, cv::Mat());
  cv::compare(image, maxMap, maxMap, cv::CMP_GE);
  cv::Mat non_plateau_mask;
  cv::erode(image, non_plateau_mask, cv::Mat());
  cv::compare(image, non_plateau_mask, non_plateau_mask, cv::CMP_GT);
  cv::bitwise_and(maxMap, non_plateau_mask, maxMap);
}

std::vector<cv::Point> FastRadialSymmetryTransform::circlePoints(int radius) const
{
  std::vector<cv::Point> result;
  int x = radius;
  int y = 0;
  int err = 0;

  //first octant
  while (x >= y)
  {
    result.push_back(cv::Point(-x,y));
    if (err <= 0)
    {
        y += 1;
        err += 2*y + 1;
    }
    if (err > 0)
    {
        x -= 1;
        err -= 2*x + 1;
    }
  }
  //second onctant (diagonal mirrored
  for(int i=result.size()-1;i>=0;i--)
  {
    addToVector(cv::Point(-result[i].y,-result[i].x),result);
  }
  //3rd and 4th octant (y-axis mirrored)
  for(int i=result.size()-1;i>=0;i--)
  {
    addToVector(cv::Point(-result[i].x,result[i].y),result);
  }
  //octant 5 to 8 (x-axis mirrored)
  for(int i=result.size()-1;i>=0;i--)
  {
    addToVector(cv::Point(result[i].x,-result[i].y),result);
  }
  return result;
}

void FastRadialSymmetryTransform::addToVector(cv::Point point, std::vector<cv::Point> &vector) const
{
  if(vector.back()!=point && vector.front()!=point)
    vector.push_back(point);
}

int FastRadialSymmetryTransform::findRadius(const std::vector<cv::Mat>& Sn, const cv::Point &p) const
{
  size_t maxIndex = 0;
  float maxValue = -100000000;
  for(size_t i=0;i<Sn.size();i++)
  {
    float value = Sn[i].at<float>(p)*ranges[i];
    if(value>maxValue)
    {
      maxValue = value;
      maxIndex = i;
    }
  }
  return ranges[maxIndex];
  /*
  for(size_t radius=ranges.size()/2;radius<ranges.size();radius++)
  {
    float meanMagnitude = 0.0;
    size_t ctr = 0;
    std::vector<cv::Point> points = circlePoints(ranges[radius]);
    for(size_t pIndex=0;pIndex<points.size();pIndex++)
    {
      cv::Point cp = p + points[pIndex];
      if(pointIsOk(cp,gradientMagnitude))
      {
        meanMagnitude += gradientMagnitude.at<float>(cp);
        ctr++;
      }
    }
    meanMagnitude /= ctr;
    if(meanMagnitude>maxMeanMagnitude)
    {
      maxMeanMagnitude = meanMagnitude;
      maxRadius = ranges[radius];
    }
  }
  return maxRadius;
  */
}

FastRadialSymmetryTransform::FastRadialSymmetryTransform()
{
  radialStrictness   = 2.0;
  gaussianStdDev = 5.0;
  magnitudeThreshold = 0.05;
  circleSuppressionFactor = 1.5;
}

cv::Mat FastRadialSymmetryTransform::transform(const cv::Mat &image, const cv::Mat &mask) const
{
  cv::Mat gray;
  cv::cvtColor(image,gray,CV_BGR2GRAY);
  cv::Mat result = cv::Mat::zeros(image.rows,image.cols,CV_32F);
  cv::Mat gradient;
  {
    cv::Mat sobelX, sobelY;
    cv::Sobel(gray,sobelX,CV_32F,1,0);
    cv::Sobel(gray,sobelY,CV_32F,0,1);
    cv::Mat magnitude;
    cv::Mat temp1, temp2;
    cv::pow(sobelX,2,temp1);
    cv::pow(sobelY,2,temp2);
    cv::sqrt(temp1+temp2,magnitude);
    std::vector<cv::Mat> channels;
    cv::divide(sobelX,magnitude,sobelX);
    cv::divide(sobelY,magnitude,sobelY);
    cv::threshold(magnitude,magnitude,magnitudeThreshold/1020,255,cv::THRESH_TOZERO);
    channels.push_back(sobelX);
    channels.push_back(sobelY);
    channels.push_back(magnitude);
    cv::merge(channels,gradient);
  }
  for(size_t i=0;i<ranges.size();i++)
  {
    result += transform(gradient,ranges[i],mask);
  }
  cv::Mat maxMap;
  nonMaximaSuppression(result,maxMap);
  cv::bitwise_and(maxMap,mask,maxMap);
  //maxMap.convertTo(maxMap,CV_32F);
  //result = result.mul(maxMap);
  return maxMap;
}

std::vector<Circle> FastRadialSymmetryTransform::getCircles(const cv::Mat &image, const cv::Mat &mask) const
{
  cv::Mat gray;
  cv::cvtColor(image,gray,CV_BGR2GRAY);
  cv::Mat S = cv::Mat::zeros(image.rows,image.cols,CV_32F);
  std::vector<cv::Mat> Sn;
  cv::Mat gradient;
  cv::Mat magnitude;
  {
    cv::Mat sobelX, sobelY;
    cv::Sobel(gray,sobelX,CV_32F,1,0);
    cv::Sobel(gray,sobelY,CV_32F,0,1);
    cv::Mat temp1, temp2;
    cv::pow(sobelX,2,temp1);
    cv::pow(sobelY,2,temp2);
    cv::sqrt(temp1+temp2,magnitude);
    std::vector<cv::Mat> channels;
    cv::divide(sobelX,magnitude,sobelX);
    cv::divide(sobelY,magnitude,sobelY);
    cv::threshold(magnitude,magnitude,magnitudeThreshold*1020,255,cv::THRESH_TOZERO);
    channels.push_back(sobelX);
    channels.push_back(sobelY);
    channels.push_back(magnitude);
    cv::merge(channels,gradient);
  }
  for(size_t i=0;i<ranges.size();i++)
  {
    Sn.push_back(transform(gradient,ranges[i],mask));
    S += Sn[Sn.size()-1];
  }
  //extract points
  cv::Mat maxMap;
  nonMaximaSuppression(S,maxMap);
  cv::bitwise_and(maxMap,mask,maxMap);
  std::vector<Circle> circles;
  std::vector<Circle> result;
  std::vector<cv::Point> points;
  cv::findNonZero(maxMap,points);
  for(size_t i=0;i<points.size();i++)
    circles.push_back(Circle(points[i],0,S.at<float>(points[i])));
  std::sort(circles.begin(),circles.end());
  //find radii
  {
    cv::Mat blockedMask = cv::Mat::zeros(image.rows,image.cols,CV_8U);
    for(size_t i=0;i<circles.size();i++)
    {
      if(blockedMask.at<uchar>(circles[i].center)==0)
      {
        circles[i].radius = findRadius(Sn,circles[i].center);
        cv::circle(blockedMask,circles[i].center,circles[i].radius*circleSuppressionFactor,cv::Scalar(255,55,255),-1);
        result.push_back(circles[i]);
      }
    }
  }
  return result;
}
