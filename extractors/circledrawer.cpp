#include "circledrawer.h"

std::vector<cv::Point> CircleDrawer::circlePoints(int radius)
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

void CircleDrawer::addToVector(cv::Point point, std::vector<cv::Point> &vector)
{
  if(vector.back()!=point && vector.front()!=point)
    vector.push_back(point);
}

CircleDrawer::CircleDrawer(int minRadius, int maxRadius)
{
  this->minRadius = minRadius;
  this->maxRadius = maxRadius;
  partialPortionRad = M_PI/8.0;
  for(int i=minRadius;i<=maxRadius;i++)
  {
    std::vector<cv::Point> points = circlePoints(i);

    drawPoints.push_back(points);
  }
}

void CircleDrawer::draw(Circle circle, cv::Mat &image)
{
  if(circle.radius<minRadius || circle.radius>maxRadius)
    return;
  size_t index = circle.radius - minRadius;
  for(size_t i=0;i<drawPoints[index].size();i++)
  {
    cv::Point p = drawPoints[index][i] + circle.center;
    if(p.x >= 0 &&
       p.x <  image.cols &&
       p.y >= 0 &&
       p.y <  image.rows)
    {
      image.at<unsigned char>(p) = 255;
    }
  }
}

void CircleDrawer::drawPartial(Circle circle, float directionRad, cv::Mat &image)
{
  if(circle.radius<minRadius || circle.radius>maxRadius)
    return;
  size_t index = circle.radius - minRadius;
  int indexDeriv = std::ceil(drawPoints[index].size()*partialPortionRad/(2*M_PI));
  {
    int directionIndex = drawPoints[index].size()*(M_PI - directionRad)/(2*M_PI);
    for(int i=directionIndex-indexDeriv+drawPoints[index].size();i<directionIndex+indexDeriv+drawPoints[index].size();i++)
    {
      cv::Point p = drawPoints[index][i%drawPoints[index].size()] + circle.center;
      if(p.x >= 0 &&
         p.x <  image.cols &&
         p.y >= 0 &&
         p.y <  image.rows)
      {
        image.at<float>(p) += 1;
      }
    }
  }
  {
    float directionRad2 = directionRad + M_PI;
    if(directionRad2>=2*M_PI)
      directionRad2 -= 2*M_PI;
    int directionIndex = drawPoints[index].size()*(M_PI - directionRad2)/(2*M_PI);
    for(int i=directionIndex-indexDeriv+drawPoints[index].size();i<directionIndex+indexDeriv+drawPoints[index].size();i++)
    {
      cv::Point p = drawPoints[index][i%drawPoints[index].size()] + circle.center;
      if(p.x >= 0 &&
         p.x <  image.cols &&
         p.y >= 0 &&
         p.y <  image.rows)
      {
        image.at<float>(p) += 1;
      }
    }
  }
}

size_t CircleDrawer::numPoints(int radius)
{
  if(radius<minRadius || radius>maxRadius)
    return 0;
  return drawPoints[radius - minRadius].size();
}
