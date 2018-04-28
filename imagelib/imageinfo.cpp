#include "imageinfo.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const QString ImageInfo::radSectionString = "radSections";
const QString ImageInfo::pointSectionString = "pointSections";
const QString ImageInfo::testDataString = "testdata";
const QString ImageInfo::ignoreString = "ignore";

QString ImageInfo::getFilepath() const
{
  return filepath;
}

void ImageInfo::setFilepath(const QString &value)
{
  filepath = value;
}

QString ImageInfo::getFilename() const
{
  return getFilepath().split("/").last();
}

cv::Mat ImageInfo::getMap() const
{
  cv::Mat image = cv::imread(getFilepath().toStdString());
  cv::Mat map = cv::Mat::zeros(image.rows,image.cols,CV_8U);
  std::vector< std::vector<cv::Point> > ppoints;
  foreach(const ImageSection& section,sections)
  {
    QPolygon poly = section.getPolygon();
    std::vector<cv::Point> points = qPointsToCvPoints(poly);
    ppoints.push_back(points);
  }
  cv::fillPoly(map,ppoints,cv::Scalar(255));
  return map;
}

cv::Mat ImageInfo::getImage() const
{
  cv::Mat image = cv::imread(getFilepath().toStdString());
  return image;
}

std::vector<cv::Point> ImageInfo::getPoints() const
{
  std::vector<cv::Point> points;
  foreach(const ImagePointSection& section,pointSections)
  {
    QPoint qp = section.getCenter();
    points.push_back(cv::Point(qp.x(),qp.y()));
  }
  return points;
}

bool ImageInfo::isInTestSet() const
{
  return isTestData() && !getIgnore();
}

bool ImageInfo::isInTrainingSet() const
{
  return isTrainingData() && !getIgnore();
}

bool ImageInfo::isTrainingData() const
{
  return !isTestData();
}

bool ImageInfo::isTestData() const
{
  return testData;
}

void ImageInfo::setTestData(bool value)
{
  testData = value;
}

bool ImageInfo::getIgnore() const
{
  return ignore;
}

void ImageInfo::setIgnore(bool value)
{
  this->ignore = value;
}

std::vector<cv::Point> ImageInfo::qPointsToCvPoints(const QPolygon &polygon) const
{
  std::vector<cv::Point> points;
  for(int i=0;i<polygon.size();i++)
  {
    points.push_back(cv::Point(polygon[i].x(),polygon[i].y()));
  }
  return points;
}

ImageInfo::ImageInfo(const QString &filepath)
{
  ignore = false;
  testData = false;
  this->filepath = filepath;
}

ImageInfo::ImageInfo(const QJsonObject &json)
{
  this->ignore = json[ignoreString].toBool(false);
  this->filepath = json["filepath"].toString();
  this->testData = json[testDataString].toBool(false);
  {
    QJsonArray sectionsArray = json["sections"].toArray();
    for(int i=0;i<sectionsArray.size();i++)
    {
      ImageSection sec(sectionsArray[i].toObject());
      if(!sec.isEmpty())
        sections.append(sec);
    }
  }
  {
    QJsonArray radSectionArray = json[radSectionString].toArray();
    for(int i=0;i<radSectionArray.size();i++)
    {
      ImageRadialSection radSec(radSectionArray[i].toObject());
      if(radSec.isValid())
      {
        radSections.append(radSec);
      }
    }
  }
  {
    QJsonArray pointSectionArray = json[pointSectionString].toArray();
    for(int i=0;i<pointSectionArray.size();i++)
    {
      ImagePointSection pointSec(pointSectionArray[i].toObject());
      if(pointSec.isValid())
      {
        pointSections.append(pointSec);
      }
    }
  }
}

QJsonObject ImageInfo::toJson()
{
  QJsonObject result;
  result.insert("filepath",filepath);
  result.insert(testDataString,isTestData());
  result.insert(ignoreString,getIgnore());
  {
    QJsonArray sectionsArray;
    for(int i=0;i<sections.size();i++)
      sectionsArray.append(sections[i].toJson());
    result.insert("sections",sectionsArray);
  }
  {
    QJsonArray radSectionsArray;
    for(int i=0;i<radSections.size();i++)
    {
      if(radSections[i].isValid())
        radSectionsArray.append(radSections[i].toJson());
    }
    result.insert(radSectionString,radSectionsArray);
  }
  {
    QJsonArray pointSectionsArray;
    foreach(const ImagePointSection& section, pointSections)
      pointSectionsArray.append(section.toJson());
    result.insert(pointSectionString,pointSectionsArray);
  }
  return result;
}
