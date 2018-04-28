#include "circle.h"

Circle::Circle(cv::Point center, int radius, float certainty)
{
  this->center = center;
  this->radius = radius;
  this->certainty = certainty;
}

bool Circle::operator <(const Circle &other)
{
  return certainty>other.certainty;
}
