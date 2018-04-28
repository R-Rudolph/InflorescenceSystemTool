#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include "imagesection.h"
#include "imagepointsection.h"
#include "imageradialsection.h"

#include <opencv2/core.hpp>

class ImageInfo
{
  static const QString radSectionString;
  static const QString pointSectionString;
  static const QString testDataString;
  static const QString ignoreString;
  bool testData;
  bool ignore;
  QString filepath;
  QList<ImageSection> sections;
  QList<ImageRadialSection> radSections;
  QList<ImagePointSection> pointSections;
  std::vector<cv::Point> qPointsToCvPoints(const QPolygon& polygon) const;
public:
  ImageInfo(const QString& filepath = QString());
  ImageInfo(const QJsonObject& json);
  QJsonObject toJson();
  QString getFilepath() const;
  void setFilepath(const QString &value);
  QString getFilename() const;

  cv::Mat getMap() const;
  cv::Mat getImage() const;
  std::vector<cv::Point> getPoints() const;

  bool isInTestSet() const;
  bool isInTrainingSet() const;
  bool isTrainingData() const;
  bool isTestData() const;
  void setTestData(bool value);
  bool getIgnore() const;
  void setIgnore(bool value);
};

#endif // IMAGEINFO_H
